/*
Author: Donavan Sirois
Date: March 16, 2026
File name: collisionmanager.h
Description: Class for the collision handling of the game
Modifications:
*/

#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "geostructs.h"
#include "utilities.h"
#include "actor.h"

class CollisionManager
{
public:
    /*
    Author: Donavan Sirois
    Date: March 16, 2026
    Description: These functions serve to do collision detection and collision maths in the game.
                We first do a distance check to see if the actor is acctually in range of colliding
                with the walls. Then, we handle the collision, drawing a normal vector to the wall and
                adjusting the player's position according to its depth of prenetration into the wall.
    */
    void narrowingToCollide(std::vector<Linedef>& broadedWalls, const std::vector<Vertex>& verteces, Actor* a);
private:
    void collide(Vertex closestPoint, Actor* a);
};

#endif
