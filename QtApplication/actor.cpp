/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Player.cpp
Goal: Code for the Player class managing player attributes and actions.
Modifications:
*/

#include "actor.h"


Actor::Actor()
{
    position = {0, 0};
    angle = 0.0f;
    health = 100;
    isAlive = true;
}

Actor::~Actor()
{

}

void Actor::move(float deltaX, float deltaY)
{
    position.x += deltaX;
    position.y += deltaY;
}

void Actor::shoot()
{
    // Shooting logic here
}

void Actor::takeDamage(int amount)
{
    health -= amount;
    if (health < 0)
    {
        health = 0;
        isAlive = false;
    }
}

int Actor::getHealth()
{
    return health;
}

float Actor::getAngle() const
{
    return angle;
}

Point2D Actor::getPosition() const
{
    return position;
}

void Actor::setAngle(float a)
{
    angle = a;
}

void Actor::rotate(float a)
{
    angle += a;
}

void Actor::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;
}

//Enemy

float Actor::distancePlayerEnemy(Actor E, Actor P)
{
    dx= P.getPosition().x-E.getPosition().x;
    dy = P.getPosition().y-E.getPosition().y;

    return std::sqrt(dx*dx + dy*dy);
}

void Actor::moveEnemy(Actor Enemy, Actor Player)
{
    float distance = distancePlayerEnemy(E,P);

    if(distance < EnemyRange)
    {
        dx = Player.getPosition().x - Enemy.getPosition().x;
        dy = Enemy.getPosition().x - Enemy.getPosition().y;

        Enemy.getAngle() = std::atan2(dy,dx);

        if(distance < 0,01f)
        {
            float nx = dx/distance;
            float ny = dy/distance;

            enemy.move(nx*EnemySpeed,ny*EnemySpeed);

        }
    }

}

