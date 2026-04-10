/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Player.h
Goal: Header file for the Player class managing player attributes and actions.
Modifications:
*/

#ifndef ACTOR_H
#define ACTOR_H
#include "geostructs.h"
#include "Weapon.h"

class Actor{
private:
    Vertex position;
    int health;
    bool isAlive;
    float angle;
    float EnemyRange = 40.0f;
    float EnemySpeed = 10.0f;
    float dx = 0;
    float dy = 0;
    bool canMove = true;
    Weapon* m_weapon = nullptr;
      int score = 0;

public:
    Actor();
    ~Actor();
    void move(float deltaX, float deltaY);
    void takeDamage(int amount);

    // Access to attributes
    int getHealth() {return health;}
    void setHealth(int h) { health = h; }
    float getAngle() const {return angle;}
    Vertex getPosition() const {return position;}
    void setPosition(Vertex newPos);
    void setAngle(float newAngle);
    void rotate(float deltaAngle);
    void setPosition(float x, float y);
    void setMovement(bool mouvement);
    float distancePlayerEnemy(const Actor& E, const Actor& P);
    void moveEnemy(const Actor& P, float deltaTime);
    void setWeapon(Weapon* w);
    Weapon* getWeapon() const;
    bool hasWeapon() const;
    void resetPlayerHealth();
    int getScore() const { return score; }
    void addScore(int points) { score += points; }
    void resetScore() { score = 0; }
     bool isMoving() const { return canMove; }
};

#endif
