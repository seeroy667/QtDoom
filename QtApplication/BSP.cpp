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

Node* BSP::Builder(std::vector<Linedef> segments)
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
        float deltaXSegmentEnd = segments[i].end.x - node->partition.start.x;
        float deltaYSegmentEnd = segments[i].end.y - node->partition.start.y;
        float deltaXSegmentStart = segments[i].start.x - node->partition.start.x;
        float deltaYSegmentStart = segments[i].start.y - node->partition.start.y;
        float deltaXPartition = node->partition.end.x - node->partition.start.x;
        float deltaYPartition = node->partition.end.y - node->partition.start.y;

        float crossProductEnd = (deltaXSegmentEnd * deltaYPartition) - (deltaYSegmentEnd * deltaXPartition);
        float crossProductStart = (deltaXSegmentStart * deltaYPartition) - (deltaYSegmentStart * deltaXPartition);

        if (crossProductEnd > 0 && crossProductStart > 0)
        { // is in front (arbitrarilly), i.e. all point of the line are in front of the partition line
            frontLines.push_back(segments[i]);
        }
        else if (crossProductEnd < 0 && crossProductStart < 0)
        { // is at the back (arbitrarilly), i.e. all the points of the line are at the back of the partition line
            backLines.push_back(segments[i]);
        }
        else // is split by the segment
        {
            // Let's create a new point to divide the segment into a front one and a back one
            Vertex intersection;

            float dxSeg = segments[i].end.x - segments[i].start.x;
            float dySeg = segments[i].end.y - segments[i].start.y;
            float dxPar = node->partition.end.x - node->partition.start.x;
            float dyPar = node->partition.end.y - node->partition.start.y;

            if (dxSeg == 0) // vertical segment case
            {
                intersection.x = segments[i].start.x;
                if (dxPar != 0) intersection.y = node->partition.start.y + (intersection.x - node->partition.start.x) * (dyPar / dxPar); // vertical partition case
                else intersection.y = segments[i].start.y;
            }
            else if (dxPar == 0) // vertical partition case
            {
                intersection.x = node->partition.start.x;
                intersection.y = segments[i].start.y + (intersection.x - segments[i].start.x) * dySeg / dxSeg;
            }
            else
            {
                float slopeSeg = dySeg/dxSeg;
                float slopePar = dyPar/dyPar;

                float bSeg = segments[i].end.y - (slopeSeg*segments[i].end.x);
                float bPar = node->partition.end.y - (slopePar*node->partition.end.x);

                // Now, we find the intersection point.
                //This could be done from the biginning, but for readability, we created new variables.
                intersection.x = (bSeg - bPar) / (slopePar - slopeSeg);
                intersection.y = (slopeSeg*intersection.x) + bSeg;
            }

            // Now, we just devide the segment with the two points and we push.
            Linedef segA = {segments[i].start, intersection, segments[i].floorHeight, segments[i].ceilingHeight};
            Linedef segB = {intersection, segments[i].end, segments[i].floorHeight, segments[i].ceilingHeight};

            if (crossProductStart > 0)
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
    node->front = Builder(frontLines);
    node->back = Builder(backLines);
    return node;
}

void BSP::traverse(const Vertex& playerPosition, std::vector<Linedef>& renderedWalls)
{
    renderedWalls.clear();
    traverseNode(root, playerPosition, renderedWalls);
}

void BSP::traverseNode(Node* node, const Vertex& playerPosition, std::vector<Linedef>& walls)
{
    if (!node) return;

    float dx = node->partition.end.x - node->partition.start.x;
    float dy = node->partition.end.y - node->partition.start.y;
    float px = playerPosition.x - node->partition.start.x;
    float py = playerPosition.y - node->partition.start.y;

    float cross = dx * py - dy * px;
    if (cross < 0)
    {
        traverseNode(node->back, playerPosition, walls);
        walls.push_back(node->partition);
        traverseNode(node->front, playerPosition, walls);
    }
    else
    {
        traverseNode(node->front, playerPosition, walls);
        walls.push_back(node->partition);
        traverseNode(node->back, playerPosition, walls);
    }
}

void BSP::build(const std::vector<Linedef>& segments)
{
    delete root;
    root = Builder(segments);
}
