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


