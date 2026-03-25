/*
Author name: Donavan Sirois
Date: Febuary 1, 2026
File name: BSP.h
Description: Header file for the Binary Space Partitioning tree implementation.
Modifications:
*/

#ifndef BSP_H
#define BSP_H

#include<QDebug>

#include"geostructs.h"

struct Node
{
    Linedef partition;
    Node* front;
    Node* back;
};

class BSP
{
public:
    BSP();
    Node* Builder(std::vector<Linedef> segments, std::vector<Vertex>& verteces);
    void traverse(const Vertex& playerPosition, std::vector<Linedef>& renderedWalls, const std::vector<Vertex>& verteces);
    void traverseNode(Node* node, const Vertex& playerPos, std::vector<Linedef>& walls, const std::vector<Vertex>& verteces);
    void build(const std::vector<Linedef>& segments, std::vector<Vertex>& verteces);

    void broadWall(Node* node, const Vertex& playerPosition, std::vector<Linedef>& broadedWalls, const std::vector<Vertex>& verteces);
    void actorToWallBroading(const Vertex& actorPosition, std::vector<Linedef>& broadedWalls, const std::vector<Vertex>& verteces);

    Node* getRoot() const { return root; };

    std::vector<Vertex> collectValidSpawnPoints(const std::vector<Vertex>& verteces,
                                                float minDistToWall);

private:
    Node* root;
    bool isPointInsideMap(const Vertex& point,
                          const std::vector<Vertex>& verteces);

    void collectAllWalls(Node* node, std::vector<Linedef>& walls);

    void collectSpawnCandidates(Node* node,
                                const std::vector<Vertex>& verteces,
                                float minDistToWall,
                                std::vector<Vertex>& candidates);

    float distancePointToSegment(const Vertex& point,
                                 const Vertex& segStart,
                                 const Vertex& segEnd);

    bool isFarEnoughFromAllWalls(const Vertex& candidate,
                                 Node* node,
                                 const std::vector<Vertex>& verteces,
                                 float minDist);
};

#endif
