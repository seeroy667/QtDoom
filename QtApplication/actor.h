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
#include <cmath>

class Actor {
private:
    Point2D position;
    int health;
    bool isAlive;
    float angle;
    float EnemyRange = 4.0f;
    float EnemySpeed = 2.0f;
    float dx = 0;
    float dy = 0;
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

    //Enemy
    float distancePlayerEnemy(const Actor& E, const Actor& P);
    void moveEnemy(Actor& E, const Actor& P);

};

#endif
