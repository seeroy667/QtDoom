/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Player.h
Goal: Header file for the Player class managing player attributes and actions.
Modifications:
*/

#ifndef ACTOR_H
#define ACTOR_H

#include"geostructs.h"

class Actor {
private:
    Point2D position;
    int health;
    bool isAlive;
    float angle;
public:
    Actor();
    ~Actor();
    void move(float deltaX, float deltaY);
    void shoot();
    void takeDamage(int amount);


    int getHealth();
    float getAngle() const;
    Point2D getPosition() const;

    void setAngle(float newAngle);
    void rotate(float deltaAngle);
    void setPosition(float x, float y);
};

#endif
