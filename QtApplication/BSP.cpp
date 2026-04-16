/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: BSP.cpp
Description: Code for the Binary Space Partitioning tree implementation.
Modifications:
    Date: April 9, 2026
        Author: Donavan Sirois
        Description: Added more readability to the class
*/

#include "bsp.h"

BSP::BSP()
{
    root = nullptr;
}

void BSP::build(const std::vector<Linedef>& segments, std::vector<Vertex>& vertices)
{
    // Safety verifications, although they should never happen within the scope of this software
    if (segments.empty() || vertices.empty())
    {
        qDebug() << "ERROR: BSP cannot be built, segment or vertices list is empty";
        qDebug() << segments.size();
        qDebug() << vertices.size();
        return;
    }

    delete root;
    root = Builder(segments, vertices);
}

Node* BSP::Builder(std::vector<Linedef> segments, std::vector<Vertex>& vertices)
{
    // Safety verifications, although they should never happen within the scope of this software
    if (segments.empty() || vertices.empty())
    {
        qDebug() << "ERROR: BSP cannot be built, segment or vertices list is empty";
        return nullptr;
    }

    // Exit statement of recursivity. When we reach the very last wall, it is added to this node, and the childs are nullptr
    if (segments.size() == 1)
    {
        Node* node = new Node();
        node->partition = segments[0];
        node->front = nullptr;
        node->back = nullptr;
        return node;
    }

    // Defining the partition. This could be chosen optimally, but for
    // the scope of this project, we just take the first segment of the list.
    Node* node = new Node();
    node->partition = segments[0];

    // Vector creation for storing childs of the node (segments that have not been partitioned yet)
    std::vector<Linedef> frontLines;
    std::vector<Linedef> backLines;


    // We iterate for every wall, we classify it if ever it is in front or behind.
    for (int i = 1; i < segments.size(); i++) // Starting at 1, since we use 0 as the partition
    {
        // We make the cross product to evaluate the position of a line compared to the partition
        float crossProductEnd = crossProduct(vertices[node->partition.end], vertices[node->partition.start], vertices[segments[i].end]);
        float crossProductStart = crossProduct(vertices[node->partition.end], vertices[node->partition.start], vertices[segments[i].start]);

        if (crossProductEnd <= 0 && crossProductStart <= 0)
        { // is in front (chosen arbitrarilly), i.e. all point of the line are in front of the partition line
            frontLines.push_back(segments[i]);
        }
        else if (crossProductEnd >= 0 && crossProductStart >= 0)
        { // is at the back (chosen arbitrarilly), i.e. all the points of the line are at the back of the partition line
            backLines.push_back(segments[i]);
        }
        else // is split by the segment. In this case, we need to subdivide the segments into smaller ones.
        {
            // Let's create a new point to divide the segment into a front one and a back one
            Vertex intersection;

            float dxSeg = vertices[segments[i].end].x - vertices[segments[i].start].x;
            float dySeg = vertices[segments[i].end].y - vertices[segments[i].start].y;
            float dxPar = vertices[node->partition.end].x - vertices[node->partition.start].x;
            float dyPar = vertices[node->partition.end].y - vertices[node->partition.start].y;

            if (dxSeg == 0) // vertical segment case
            {
                intersection.x = vertices[segments[i].start].x;
                intersection.y = vertices[node->partition.start].y +
                                 (intersection.x - vertices[node->partition.start].x) * (dyPar / dxPar);
            }
            else if (dxPar == 0) // vertical partition case
            {
                intersection.x = vertices[node->partition.start].x;
                intersection.y = vertices[segments[i].start].y + (intersection.x - vertices[segments[i].start].x) * dySeg / dxSeg;
            }
            else // We find the equation for the segment
            {
                float slopeSeg = dySeg/dxSeg;
                float slopePar = dyPar/dxPar;

                float bSeg = vertices[segments[i].end].y - (slopeSeg*vertices[segments[i].end].x);
                float bPar = vertices[node->partition.end].y - (slopePar*vertices[node->partition.end].x);

                // Now, we find the intersection point by equalizing them
                intersection.x = (bSeg - bPar) / (slopePar - slopeSeg);
                intersection.y = (slopeSeg*intersection.x) + bSeg;
            }

            std::vector<Vertex>::iterator found = vertices.end();

            for (std::vector<Vertex>::iterator i = vertices.begin(); i != vertices.end(); i++)
            {
                const Vertex& v = *i; // Could replace the type by iterator

                float dx = v.x - intersection.x;
                float dy = v.y - intersection.y;

                bool sameX = (dx == 0);
                bool sameY = (dy == 0);

                if (sameX && sameY)
                {
                    found = i;
                    break;
                }
            }

            int vertexIndex = 0;
            if (found != vertices.end()) {
                vertexIndex = std::distance(vertices.begin(), found);
            } else {
                vertices.push_back(intersection);
                vertexIndex = vertices.size() - 1;
            }

            // Now, we just devide the segment with the two points and we push.
            Linedef segA = {segments[i].start, vertexIndex, segments[i].sideFront, segments[i].sideBack, segments[i].twoSided};
            Linedef segB = {vertexIndex, segments[i].end, segments[i].sideFront, segments[i].sideBack, segments[i].twoSided};

            // If the starting point of the segment is in the back, this means that we push back the first segment and front the second.
            // else, we do the oposite
            if (crossProductStart <= 0)
            {
                frontLines.push_back(segA);
                backLines.push_back(segB);
            }
            else
            {
                backLines.push_back(segA);
                frontLines.push_back(segB);
            }
        }
    }

    // Recursively keep building the subtree with the front lines and the backlines.
    node->front = Builder(frontLines, vertices);
    node->back = Builder(backLines, vertices);
    return node;
}

// These functions order the walls into the vector renderedWalls. THEY DO NOT ACCOUNT FOR VIEW CULLING.
// As of the 13th of April 2026, this is not used anymore for the rendering. The method is still here just in case,
// but is never used. It can be usefull to understand tree parsing, though.
void BSP::traverse(const Vertex& playerPosition,
                   std::vector<Linedef>& renderedWalls,
                   const std::vector<Vertex>& vertices)
{
    renderedWalls.clear();
    traverseNode(root, playerPosition, renderedWalls, vertices);
}

void BSP::traverseNode(Node* node, const Vertex& playerPosition,
                       std::vector<Linedef>& walls,
                       const std::vector<Vertex>& vertices)
{
    if (!node) return;

    float dxPartition = vertices[node->partition.end].x - vertices[node->partition.start].x;
    float dyPartition = vertices[node->partition.end].y - vertices[node->partition.start].y;
    float dxPlayer = playerPosition.x - vertices[node->partition.start].x;
    float dyPlayer = playerPosition.y - vertices[node->partition.start].y;

    float cross = dxPartition * dyPlayer - dyPartition * dxPlayer;

    if (cross < 0) // Front to back
    {
        traverseNode(node->back, playerPosition, walls, vertices);
        walls.push_back(node->partition);
        traverseNode(node->front, playerPosition, walls, vertices);
    }
    else
    {
        traverseNode(node->front, playerPosition, walls, vertices);
        walls.push_back(node->partition);
        traverseNode(node->back, playerPosition, walls, vertices);
    }
}

void BSP::traverseAndRender(Node* node,
                            const Vertex& playerPosition,
                            const std::vector<Vertex>& vertices,
                            std::function<bool(const Linedef&)> callback)
{
    if (!node) return;

    float cross = crossProduct(vertices[node->partition.start], vertices[node->partition.end], playerPosition);

    Node* nearChild;
    Node* farChild;

    if (cross < 0) // Front to back
    {
        nearChild = node->back;
        farChild = node->front;
    }
    else
    {
        nearChild = node->front;
        farChild = node->back;
    }

    traverseAndRender(nearChild, playerPosition, vertices, callback);

    if (!callback(node->partition)) return; // screen full, stop

    traverseAndRender(farChild, playerPosition, vertices, callback);
}

// This is used in collision detection
void BSP::actorToWallBroading(const Vertex& actorPosition, std::vector<Linedef>& broadedWalls, const std::vector<Vertex>& vertices)
{
    broadedWalls.clear();
    broadWall(root, actorPosition, broadedWalls, vertices);
}

void BSP::broadWall(Node* node, const Vertex& playerPosition, std::vector<Linedef>& broadedWalls, const std::vector<Vertex>& vertices)
{
    if (!node) return;

    float radius = 2.0f;

    float dxPartition = vertices[node->partition.end].x - vertices[node->partition.start].x;
    float dyPartition = vertices[node->partition.end].y - vertices[node->partition.start].y;
    float dxPlayer = playerPosition.x - vertices[node->partition.start].x;
    float dyPlayer = playerPosition.y - vertices[node->partition.start].y;

    float cross = dxPlayer * dyPartition - dyPlayer * dxPartition;

    // Here, we decide to not only broad the wall, but to do some volume culling.
    // We check if the player is at a reasonable distance of the wall. If he is, we add it to the valid walls.
    // This is called bounding volume hierarchy (BVH).
    // It is furthermore usefull with BSP, as it eliminates having to pass through the children of a given node.

    float wallLength = sqrt(dxPartition * dxPartition + dyPartition * dyPartition);
    float distance = cross / wallLength;

    broadedWalls.push_back(node->partition);

    if (distance < -radius)
    {
        broadWall(node->front, playerPosition, broadedWalls, vertices);
    }
    else if (distance > radius)
    {
        broadWall(node->back, playerPosition, broadedWalls, vertices);
    }
    else
    {
        broadWall(node->front, playerPosition, broadedWalls, vertices);
        broadWall(node->back, playerPosition, broadedWalls, vertices);
    }
}

bool BSP::enemyRendering(const Vertex& playerPosition, const Vertex& enemyPosition, const std::vector<Vertex>& vertices)
{
    return enemyRenderingCheck(root, playerPosition, enemyPosition, vertices);
}

bool BSP::enemyRenderingCheck(Node* node, const Vertex& playerPosition, const Vertex& enemyPosition, const std::vector<Vertex>& vertices)
{
    if (!node) return true;

    float rxCenter = enemyPosition.x - playerPosition.x;
    float ryCenter = enemyPosition.y - playerPosition.y;

    float rxRight = enemyPosition.x - playerPosition.x;
    float ryRight = enemyPosition.y - playerPosition.y;

    float rxLeft = enemyPosition.x - playerPosition.x;
    float ryLeft = enemyPosition.y - playerPosition.y;

    float sx = vertices[node->partition.end].x - vertices[node->partition.start].x;
    float sy = vertices[node->partition.end].y - vertices[node->partition.start].y;

    float cross = rxCenter * sy - ryCenter * sx;

    if (std::abs(cross) > 0)
    {
        float dx = vertices[node->partition.start].x - playerPosition.x;
        float dy = vertices[node->partition.start].y - playerPosition.y;

        float t = (dx * sy - dy * sx) / cross;
        float s = (dx * ryCenter - dy * rxCenter) / cross;

        if (t >= 0.0f && t <= 1.0f && s >= 0.0f && s <= 1.0f) return false;
    }

    cross = crossProduct(vertices[node->partition.start], vertices[node->partition.end], playerPosition);
    if (!enemyRenderingCheck(node->front, playerPosition, enemyPosition, vertices))
        return false;

    return enemyRenderingCheck(node->back, playerPosition, enemyPosition, vertices);
}

float BSP::distancePointToSegment(const Vertex& point,const Vertex& segStart,const Vertex& segEnd)
{
    float dx = segEnd.x - segStart.x;
    float dy = segEnd.y - segStart.y;
    float lengthSq = dx*dx + dy*dy;

    if(lengthSq < 0.00001f)
    {
        float ex = point.x - segStart.x;
        float ey = point.y - segStart.y;
        return std::sqrt(ex*ex + ey*ey);
    }
    float t = ((point.x - segStart.x)*dx + (point.y - segStart.y)*dy) / lengthSq;

    float closestX = segStart.x + t * dx;
    float closestY = segStart.y + t * dy;

    float ex = point.x - closestX;
    float ey = point.y - closestY;
    return std::sqrt(ex*ex + ey*ey);
}

bool BSP::isFarEnoughFromAllWalls(const Vertex& candidate,
                                  Node* node,
                                  const std::vector<Vertex>& vertices,
                                  float minDist)
{
    if (!node) return true;

    const Vertex& wStart = vertices[node->partition.start];
    const Vertex& wEnd   = vertices[node->partition.end];

    float dist = distancePointToSegment(candidate, wStart, wEnd);
    if (dist < minDist)
        return false;


    float dx    = wEnd.x - wStart.x;
    float dy    = wEnd.y - wStart.y;
    float cross = (candidate.x - wStart.x) * dy - (candidate.y - wStart.y) * dx;

    if (cross <= 0)
        return isFarEnoughFromAllWalls(candidate, node->front, vertices, minDist);
    else
        return isFarEnoughFromAllWalls(candidate, node->back, vertices, minDist);
}


bool BSP::isPointInsideMap(const Vertex& point,
                           const std::vector<Vertex>& vertices)
{
    std::vector<Linedef> allWalls;
    collectAllWalls(root, allWalls);

    int intersections = 0;

    for (const Linedef& wall : allWalls)
    {
        const Vertex& v1 = vertices[wall.start];
        const Vertex& v2 = vertices[wall.end];

        // Rayon vers +X depuis le point
        // Le mur doit traverser la hauteur Y du point
        if ((v1.y <= point.y && v2.y > point.y) ||
            (v2.y <= point.y && v1.y > point.y))
        {
            // X d'intersection du mur avec le rayon horizontal
            float t = (point.y - v1.y) / (v2.y - v1.y);
            float intersectX = v1.x + t * (v2.x - v1.x);

            if (intersectX > point.x)
                intersections++;
        }
    }

    // Impair = intérieur, pair = extérieur
    return (intersections % 2) == 1;
}

void BSP::collectAllWalls(Node* node, std::vector<Linedef>& walls)
{
    if (!node) return;
    walls.push_back(node->partition);
    collectAllWalls(node->front, walls);
    collectAllWalls(node->back, walls);
}
void BSP::collectSpawnCandidates(Node* node,
                                 const std::vector<Vertex>& vertices,
                                 float minDistToWall,
                                 std::vector<Vertex>& candidates)
{
    if (!node) return;

    const Vertex& wStart = vertices[node->partition.start];
    const Vertex& wEnd   = vertices[node->partition.end];

    float dx  = wEnd.x - wStart.x;
    float dy  = wEnd.y - wStart.y;
    float len = std::sqrt(dx*dx + dy*dy);

    if (len > 0.0001f)
    {
        float nx = -dy / len;
        float ny =  dx / len;
        float offset = minDistToWall * 2.0f;


        std::vector<float> tValues = {0.25f, 0.5f, 0.75f};

        for (float t : tValues)
        {
            Vertex wallPoint = {
                wStart.x + t * dx,
                wStart.y + t * dy
            };


            Vertex frontCandidate = { wallPoint.x + nx * offset,
                                     wallPoint.y + ny * offset };

            Vertex backCandidate  = { wallPoint.x - nx * offset,
                                    wallPoint.y - ny * offset };

            if (isPointInsideMap(frontCandidate, vertices) &&
                isFarEnoughFromAllWalls(frontCandidate, root, vertices, minDistToWall))
            {

                bool tooClose = false;
                for (const Vertex& existing : candidates)
                {
                    float ex = existing.x - frontCandidate.x;
                    float ey = existing.y - frontCandidate.y;
                    if ((ex*ex + ey*ey) < (minDistToWall * minDistToWall))
                    {
                        tooClose = true;
                        break;
                    }
                }
                if (!tooClose)
                    candidates.push_back(frontCandidate);
            }

            if (isPointInsideMap(backCandidate, vertices) &&
                isFarEnoughFromAllWalls(backCandidate, root, vertices, minDistToWall))
            {
                bool tooClose = false;
                for (const Vertex& existing : candidates)
                {
                    float ex = existing.x - backCandidate.x;
                    float ey = existing.y - backCandidate.y;
                    if ((ex*ex + ey*ey) < (minDistToWall * minDistToWall))
                    {
                        tooClose = true;
                        break;
                    }
                }
                if (!tooClose)
                    candidates.push_back(backCandidate);
            }
        }
    }

    collectSpawnCandidates(node->front, vertices, minDistToWall, candidates);
    collectSpawnCandidates(node->back,  vertices, minDistToWall, candidates);
}
std::vector<Vertex> BSP::collectValidSpawnPoints(const std::vector<Vertex>& vertices,
                                                 float minDistToWall)
{
    std::vector<Vertex> candidates;
    collectSpawnCandidates(root, vertices, minDistToWall, candidates);
    qDebug() << "BSP:" << candidates.size() << "spawn points valides";
    return candidates;
}

bool BSP::segmentsIntersect(const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d)
{
    float dx1 = b.x - a.x;
    float dy1 = b.y - a.y;
    float dx2 = d.x - c.x;
    float dy2 = d.y - c.y;

    float denom = dx1 * dy2 - dy1 * dx2;
    if (std::abs(denom) < 0.0001f) return false; // parallèles

    float t = ((c.x - a.x) * dy2 - (c.y - a.y) * dx2) / denom;
    float s = ((c.x - a.x) * dy1 - (c.y - a.y) * dx1) / denom;

    return (t > 0.001f && t < 0.999f && s > 0.001f && s < 0.999f);
}

bool BSP::losCheck(Node* node, const Vertex& from, const Vertex& to, const std::vector<Vertex>& vertices)
{
    if (!node) return true; // pas de mur return true

    const Vertex& wStart = vertices[node->partition.start];
    const Vertex& wEnd   = vertices[node->partition.end];

    // Si le mur est est twoSided il bloque pas
    if (!node->partition.twoSided)
    {
        if (segmentsIntersect(from, to, wStart, wEnd))
            return false; //un mur donc return false
    }

    // Parcourir le reste
    if (!losCheck(node->front, from, to, vertices)) return false;
    if (!losCheck(node->back,  from, to, vertices)) return false;

    return true;
}

bool BSP::hasLineOfSight(const Vertex& from, const Vertex& to, const std::vector<Vertex>& vertices)
{
    return losCheck(root, from, to, vertices);
}
