/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: BSP.cpp
Description: Code for the Binary Space Partitioning tree implementation.
Modifications:
*/

#include "bsp.h"

BSP::BSP()
{
    root = nullptr;
}

Node* BSP::Builder(std::vector<Linedef> segments, std::vector<Vertex>& verteces)
{
    // Safety verifications
    if (segments.empty()) return nullptr;
    if (segments.size() == 1)
    {
        Node* node = new Node();
        node->partition = segments[0];
        node->front = nullptr;
        node->back = nullptr;
        return node;
    }

    // Defining the partition (later will be called optimally, right now we only use the first segment)
    Node* node = new Node();
    node->partition = segments[0];

    std::vector<Linedef> frontLines;
    std::vector<Linedef> backLines;

    for (int i = 1; i < segments.size(); i++) // Starting at 1, since we use 0 as the partition
    {

        // We make the cross product to evaluate the position of a line compared to the partition
        float deltaXSegmentEnd = verteces[segments[i].end].x - verteces[node->partition.start].x;
        float deltaYSegmentEnd = verteces[segments[i].end].y - verteces[node->partition.start].y;
        float deltaXSegmentStart = verteces[segments[i].start].x - verteces[node->partition.start].x;
        float deltaYSegmentStart = verteces[segments[i].start].y - verteces[node->partition.start].y;
        float deltaXPartition = verteces[node->partition.end].x - verteces[node->partition.start].x;
        float deltaYPartition = verteces[node->partition.end].y - verteces[node->partition.start].y;

        float crossProductEnd = (deltaXSegmentEnd * deltaYPartition) - (deltaYSegmentEnd * deltaXPartition);
        float crossProductStart = (deltaXSegmentStart * deltaYPartition) - (deltaYSegmentStart * deltaXPartition);

        if (crossProductEnd <= 0 && crossProductStart <= 0)
        { // is in front (arbitrarilly), i.e. all point of the line are in front of the partition line
            frontLines.push_back(segments[i]);
        }
        else if (crossProductEnd >= 0 && crossProductStart >= 0)
        { // is at the back (arbitrarilly), i.e. all the points of the line are at the back of the partition line
            backLines.push_back(segments[i]);
        }
        else // is split by the segment
        {
            // Let's create a new point to divide the segment into a front one and a back one
            Vertex intersection;

            float dxSeg = verteces[segments[i].end].x - verteces[segments[i].start].x;
            float dySeg = verteces[segments[i].end].y - verteces[segments[i].start].y;
            float dxPar = verteces[node->partition.end].x - verteces[node->partition.start].x;
            float dyPar = verteces[node->partition.end].y - verteces[node->partition.start].y;

            if (dxSeg == 0) // vertical segment case
            {
                intersection.x = verteces[segments[i].start].x;
                intersection.y = verteces[node->partition.start].y +
                                 (intersection.x - verteces[node->partition.start].x) * (dyPar / dxPar);
            }
            else if (dxPar == 0) // vertical partition case
            {
                intersection.x = verteces[node->partition.start].x;
                intersection.y = verteces[segments[i].start].y + (intersection.x - verteces[segments[i].start].x) * dySeg / dxSeg;
            }
            else
            {
                float slopeSeg = dySeg/dxSeg;
                float slopePar = dyPar/dxPar;

                float bSeg = verteces[segments[i].end].y - (slopeSeg*verteces[segments[i].end].x);
                float bPar = verteces[node->partition.end].y - (slopePar*verteces[node->partition.end].x);

                // Now, we find the intersection point.
                // This could be done from the biginning, but for readability, we created new variables.
                intersection.x = (bSeg - bPar) / (slopePar - slopeSeg);
                intersection.y = (slopeSeg*intersection.x) + bSeg;
            }

            auto found = std::find_if(verteces.begin(), verteces.end(),
                                      [&](const Vertex& v) {
                                          return fabs(v.x - intersection.x) < 0.001f &&
                                                 fabs(v.y - intersection.y) < 0.001f;
                                      });

            int vertexIndex = 0;
            if (found != verteces.end()) {
                vertexIndex = std::distance(verteces.begin(), found);
            } else {
                verteces.push_back(intersection);
                vertexIndex = verteces.size() - 1;
            }

            // Now, we just devide the segment with the two points and we push.
            Linedef segA = {segments[i].start, vertexIndex, segments[i].sideFront, segments[i].sideBack, segments[i].twoSided};
            Linedef segB = {vertexIndex, segments[i].end, segments[i].sideFront, segments[i].sideBack, segments[i].twoSided};

            if (crossProductStart < 0)
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
    node->front = Builder(frontLines, verteces);
    node->back = Builder(backLines, verteces);
    return node;
}

// These functions order the walls. THEY DO NOT ACCOUNT FOR VIEW CULLING YET! THIS MEANS EVERY WALL GETS RENDERED!
void BSP::traverse(const Vertex& playerPosition, std::vector<Linedef>& renderedWalls, const std::vector<Vertex>& verteces)
{
    renderedWalls.clear();
    traverseNode(root, playerPosition, renderedWalls, verteces);
}

void BSP::traverseNode(Node* node, const Vertex& playerPosition, std::vector<Linedef>& walls, const std::vector<Vertex>& verteces)
{
    if (!node) return;

    float dxPartition = verteces[node->partition.end].x - verteces[node->partition.start].x;
    float dyPartition = verteces[node->partition.end].y - verteces[node->partition.start].y;
    float dxPlayer = playerPosition.x - verteces[node->partition.start].x;
    float dyPlayer = playerPosition.y - verteces[node->partition.start].y;

    float cross = dxPartition * dyPlayer - dyPartition * dxPlayer;

    if (cross > 0)
    {
        traverseNode(node->back, playerPosition, walls, verteces);
        walls.push_back(node->partition);
        traverseNode(node->front, playerPosition, walls, verteces);
    }
    else
    {
        traverseNode(node->front, playerPosition, walls, verteces);
        walls.push_back(node->partition);
        traverseNode(node->back, playerPosition, walls, verteces);
    }
}

void BSP::build(const std::vector<Linedef>& segments, std::vector<Vertex>& verteces)
{
    delete root;
    root = Builder(segments, verteces);
}

void BSP::actorToWallBroading(const Vertex& actorPosition, std::vector<Linedef>& broadedWalls, const std::vector<Vertex>& verteces)
{
    broadedWalls.clear();
    broadWall(root, actorPosition, broadedWalls, verteces);
}

void BSP::broadWall(Node* node, const Vertex& playerPosition, std::vector<Linedef>& broadedWalls, const std::vector<Vertex>& verteces)
{
    if (!node) return;

    float radius = 2.0f;

    float dxPartition = verteces[node->partition.end].x - verteces[node->partition.start].x;
    float dyPartition = verteces[node->partition.end].y - verteces[node->partition.start].y;
    float dxPlayer = playerPosition.x - verteces[node->partition.start].x;
    float dyPlayer = playerPosition.y - verteces[node->partition.start].y;

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
        broadWall(node->front, playerPosition, broadedWalls, verteces);
    }
    else if (distance > radius)
    {
        broadWall(node->back, playerPosition, broadedWalls, verteces);
    }
    else
    {
        broadWall(node->front, playerPosition, broadedWalls, verteces);
        broadWall(node->back, playerPosition, broadedWalls, verteces);
    }
}

bool BSP::enemyRendering(const Vertex& playerPosition, const Vertex& enemyPosition, const std::vector<Vertex>& verteces)
{
    return enemyRenderingCheck(root, playerPosition, enemyPosition, verteces);
}

bool BSP::enemyRenderingCheck(Node* node, const Vertex& playerPosition, const Vertex& enemyPosition, const std::vector<Vertex>& verteces)
{
    if (!node) return true;

    float rx = enemyPosition.x - playerPosition.x;
    float ry = enemyPosition.y - playerPosition.y;

    float sx = verteces[node->partition.end].x - verteces[node->partition.start].x;
    float sy = verteces[node->partition.end].y - verteces[node->partition.start].y;

    float cross = rx * sy - ry * sx;

    if (std::abs(cross) > 0)
    {
        float dx = verteces[node->partition.start].x - playerPosition.x;
        float dy = verteces[node->partition.start].y - playerPosition.y;

        float t = (dx * sy - dy * sx) / cross;
        float s = (dx * ry - dy * rx) / cross;

        if (t >= 0.0f && t <= 1.0f && s >= 0.0f && s <= 1.0f) return false;
    }

    float dxPartition = verteces[node->partition.end].x - verteces[node->partition.start].x;
    float dyPartition = verteces[node->partition.end].y - verteces[node->partition.start].y;
    float dxPlayer = playerPosition.x - verteces[node->partition.start].x;
    float dyPlayer = playerPosition.y - verteces[node->partition.start].y;
    cross = dxPlayer * dyPartition - dyPlayer * dxPartition;

    if (!enemyRenderingCheck(node->front, playerPosition, enemyPosition, verteces))
        return false;

    return enemyRenderingCheck(node->back, playerPosition, enemyPosition, verteces);
}

float crossProduct(Vertex v, Linedef l)
{
    return 0.1f;
}

float crossProduct(Linedef l1, Linedef l2)
{
    return 0.1f;
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
                                  const std::vector<Vertex>& verteces,
                                  float minDist)
{
    if (!node) return true;

    const Vertex& wStart = verteces[node->partition.start];
    const Vertex& wEnd   = verteces[node->partition.end];

    float dist = distancePointToSegment(candidate, wStart, wEnd);
    if (dist < minDist)
        return false;


    float dx    = wEnd.x - wStart.x;
    float dy    = wEnd.y - wStart.y;
    float cross = (candidate.x - wStart.x) * dy - (candidate.y - wStart.y) * dx;

    if (cross >= 0)
        return isFarEnoughFromAllWalls(candidate, node->front, verteces, minDist);
    else
        return isFarEnoughFromAllWalls(candidate, node->back, verteces, minDist);
}


bool BSP::isPointInsideMap(const Vertex& point,
                           const std::vector<Vertex>& verteces)
{
    std::vector<Linedef> allWalls;
    collectAllWalls(root, allWalls);

    int intersections = 0;

    for (const Linedef& wall : allWalls)
    {
        const Vertex& v1 = verteces[wall.start];
        const Vertex& v2 = verteces[wall.end];

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
                                 const std::vector<Vertex>& verteces,
                                 float minDistToWall,
                                 std::vector<Vertex>& candidates)
{
    if (!node) return;

    const Vertex& wStart = verteces[node->partition.start];
    const Vertex& wEnd   = verteces[node->partition.end];

    Vertex wallMid = {
        (wStart.x + wEnd.x) / 2.0f,
        (wStart.y + wEnd.y) / 2.0f
    };

    float dx  = wEnd.x - wStart.x;
    float dy  = wEnd.y - wStart.y;
    float len = std::sqrt(dx*dx + dy*dy);

    if (len > 0.0001f)
    {
        float nx = -dy / len;
        float ny =  dx / len;
        float offset = minDistToWall * 2.0f;

        Vertex frontCandidate = { wallMid.x + nx * offset,
                                 wallMid.y + ny * offset };
        Vertex backCandidate  = { wallMid.x - nx * offset,
                                wallMid.y - ny * offset };

        if (isPointInsideMap(frontCandidate, verteces) &&
            isFarEnoughFromAllWalls(frontCandidate, root, verteces, minDistToWall))
        {
            candidates.push_back(frontCandidate);
        }

        if (isPointInsideMap(backCandidate, verteces) &&
            isFarEnoughFromAllWalls(backCandidate, root, verteces, minDistToWall))
        {
            candidates.push_back(backCandidate);
        }
    }

    collectSpawnCandidates(node->front, verteces, minDistToWall, candidates);
    collectSpawnCandidates(node->back,  verteces, minDistToWall, candidates);
}
std::vector<Vertex> BSP::collectValidSpawnPoints(const std::vector<Vertex>& verteces,
                                                 float minDistToWall)
{
    std::vector<Vertex> candidates;
    collectSpawnCandidates(root, verteces, minDistToWall, candidates);
    qDebug() << "BSP:" << candidates.size() << "spawn points valides";
    return candidates;
}
