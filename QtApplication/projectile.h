#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "geoStructs.h"

struct Projectile
{
    Vertex position;
    float dirX;
    float dirY;
    float distanceTraveled = 0.0f;
    float maxDistance = 50.0f;
    bool active = true;
};

#endif // PROJECTILE_H
