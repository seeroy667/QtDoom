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

    // The functions bellow are called by other managers, but their associated functions and descriptions are private members.
    // The function header is still the same.

    /*
    Author: Donavan Sirois
    Date: Febuary 1, 2026
    Description: Those are recursive functions which builds a BSP tree with map data.
        Each leaf is a segment of a subsegment devided by a partiion line
    Modifications:
        Date: April 10, 2026
            Author: Donavan Sirois
            Description: Added more readability to the class. Comments and function substitution
    */
    void build(const std::vector<Linedef>& segments,
               std::vector<Vertex>& vertices);

    /*
    Author: Donavan Sirois
    Date: Febuary 1, 2026
    Description: Those are recursive functions which parse through the BSP and sort the walls
                from either back-to-front or front-to-back depending on the cross product operation.
                The sorted walls can be found in the renderedWalls vector.
    Modifications:
        Date: April 10, 2026
            Author: Donavan Sirois
            Description: Added more readability to the class. Comments and function substitution
    */
    void traverse(const Vertex& playerPosition,
                  std::vector<Linedef>& renderedWalls,
                  const std::vector<Vertex>& vertices);

    /*
    Author: Donavan Sirois
    Date: April 10, 2026
    Description: This function uses an anonymous function to render walls until the callback condition is met.
                In our case, we want parse the BSP tree front-to-back in order to render walls from the first seen
                to the last one seen. Our exit condition is whether or not, in the renderManager, the columns are full.
    */
    void traverseAndRender(Node* node,
                           const Vertex& playerPosition,
                           const std::vector<Vertex>& vertices,
                           std::function<bool(const Linedef&)> callback);

    /*
    Author: Donavan Sirois
    Date: March 18, 2026
    Description: Those are recursive functions used to choose which walls to check collisions with.
                They use BHV in order to decide if an actor is in range of the wall, and we parse
                through the tree, ignoring certain children, which is way faster then checking
                for every single wall.
    */
    void actorToWallBroading(const Vertex& actorPosition,
                             std::vector<Linedef>& broadedWalls,
                             const std::vector<Vertex>& vertices);
    void broadWall(Node* node,
                   const Vertex& playerPosition,
                   std::vector<Linedef>& broadedWalls,
                   const std::vector<Vertex>& vertices);

    /*
    Author: Donavan Sirois
    Date: March 25, 2026
    Description: These functions raycast between the player and the enemy. If they find a wall intersection
                this raycast, the false value is returned and the enemy is not rendered. If, on the contrairy,
                a wall is not found, the enemy is rendered.
    */
    bool enemyRendering(const Vertex& playerPosition,
                        const Vertex& enemyPosition,
                        const std::vector<Vertex>& vertices);

    /*
    Author: Mathieu Vincent
    Date: March 25, 2026
    Description: Used to collect possible spawn points for enemies
    */
    std::vector<Vertex> collectValidSpawnPoints(const std::vector<Vertex>& vertices,
                                                float minDistToWall);

    // Member Accessibility
    Node* getRoot() const { return root; };

private:
    Node* root; // Root node of the tree

    // Used to build the tree
    Node* Builder(std::vector<Linedef> segments,
                  std::vector<Vertex>& vertices);

    // Used to traverse the tree
    void traverseNode(Node* node,
                      const Vertex& playerPos,
                      std::vector<Linedef>& walls,
                      const std::vector<Vertex>& vertices);

    // Used for rendering enemies
    bool enemyRenderingCheck(Node* node,
                             const Vertex& playerPosition,
                             const Vertex& enemyPosition,
                             const std::vector<Vertex>& vertices);
    std::vector<Vertex> collectValidSpawnPoints(const std::vector<Vertex>& vertices,
                                                float minDistToWall);
    bool hasLineOfSight(const Vertex& from, const Vertex& to, const std::vector<Vertex>& vertices);



    /*
    Author: Mathieu Vincent
    Date: March 25, 2026
    Description: Used to collect possible spawn points for enemies
    */
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

    bool losCheck(Node* node, const Vertex& from, const Vertex& to, const std::vector<Vertex>& vertices);
    bool segmentsIntersect(const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d);
};

#endif
