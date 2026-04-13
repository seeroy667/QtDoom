/*
Author name: Donavan Sirois
Date: Febuary 1, 2026
File name: BSP.h
Description: Header file for the Binary Space Partitioning tree implementation.
Modifications:
    Date: April 10, 2026
        Author: Donavan Sirois
        Description: Comments and readibility
*/

#ifndef BSP_H
#define BSP_H

#include<QDebug>

#include"geostructs.h"
#include"utilities.h"
#include"actor.h"

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

    /*
    Author: Donavan Sirois
    Date: Febuary 1, 2026
    Description: This is a recursive function which builds a BSP tree with map data.
        Each leaf is a segment of a subsegment devided by a partiion line
    Modifications:
        Date: April 10, 2026
            Author: Donavan Sirois
            Description: Added more readability to the class. Comments and function substitution
    */
    Node* Builder(std::vector<Linedef> segments,
                  std::vector<Vertex>& vertices);

    void traverse(const Vertex& playerPosition,
                  std::vector<Linedef>& renderedWalls,
                  const std::vector<Vertex>& vertices);
    void traverseNode(Node* node,
                      const Vertex& playerPos,
                      std::vector<Linedef>& walls,
                      const std::vector<Vertex>& vertices);
    void build(const std::vector<Linedef>& segments,
               std::vector<Vertex>& vertices);

    // For collision on all actors
    void broadWall(Node* node,
                   const Vertex& playerPosition,
                   std::vector<Linedef>& broadedWalls,
                   const std::vector<Vertex>& vertices);
    void actorToWallBroading(const Vertex& actorPosition,
                             std::vector<Linedef>& broadedWalls,
                             const std::vector<Vertex>& vertices);

    // For enemy rendering
    bool enemyRendering(const Vertex& playerPosition,
                        const Vertex& enemyPosition,
                        const std::vector<Vertex>& vertices);
    bool enemyRenderingCheck(Node* node,
                             const Vertex& playerPosition,
                             const Vertex& enemyPosition,
                             const std::vector<Vertex>& vertices);
    std::vector<Vertex> collectValidSpawnPoints(const std::vector<Vertex>& vertices,
                                                float minDistToWall);

    // Annexes
    Node* getRoot() const { return root; };

private:
    Node* root;
    bool isPointInsideMap(const Vertex& point,
                          const std::vector<Vertex>& vertices);

    void collectAllWalls(Node* node, std::vector<Linedef>& walls);

    void collectSpawnCandidates(Node* node,
                                const std::vector<Vertex>& vertices,
                                float minDistToWall,
                                std::vector<Vertex>& candidates);

    float distancePointToSegment(const Vertex& point,
                                 const Vertex& segStart,
                                 const Vertex& segEnd);

    bool isFarEnoughFromAllWalls(const Vertex& candidate,
                                 Node* node,
                                 const std::vector<Vertex>& vertices,
                                 float minDist);
};

#endif
