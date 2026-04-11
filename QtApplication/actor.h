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
#include <QElapsedTimer>

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

    //---Enemy Range---
      bool m_isRanged = false;
      float m_shootCooldown = 3000.0f;
      QElapsedTimer m_shootTimer;
      bool m_timerStarted = false;
      bool m_isShooting = false;
      QElapsedTimer m_shootAnimTimer;
      float m_shootAnimDuration = 0.2f;


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

    //---Enemy Range---
    bool isRanged()const { return m_isRanged;}
    void setRanged(bool r) { m_isRanged = r;}
    bool canShootProjectile();
    void triggerShootAnim(){m_isShooting = true; m_shootAnimTimer.restart();}
    bool isShooting()const {return m_isShooting;}
    float getShootAnimElapsed()const { return m_shootAnimTimer.elapsed() / 1000.0f;}
    void updateShootAnim();

};

#endif
