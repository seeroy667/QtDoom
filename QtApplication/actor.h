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
    // Geometry
    Vertex position;
    float angle;

    // Health
    int health;
    int maxHealth;

    // Status
    bool isAlive;
    bool canMove = true;
    int score = 0;

    // Weapon
    Weapon* m_weapon = nullptr;
    float EnemyRange = 40.0f;
    float EnemySpeed = 8.0f;

    //---Range Enemy Attack---
      bool m_isRanged = false;
      float m_shootCooldown = 3000.0f;
      QElapsedTimer m_shootTimer;
      bool m_timerStarted = false;
      bool m_isShooting = false;
      QElapsedTimer m_shootAnimTimer;
      float m_shootAnimDuration = 0.2f;

public:
    Actor();

    /*
    * --------------------------------------------------------------------------------
    * Reading member functions
    * --------------------------------------------------------------------------------
    */
    int getHealth() {return health;}
    void setHealth(int h) { health = h;}
    float getAngle() const {return angle;}
    Vertex getPosition() const {return position;}
    int getScore() const { return score; }
    Weapon* getWeapon() const {return m_weapon;}
    int getMaxHealth() const { return maxHealth; }

    /*
    * --------------------------------------------------------------------------------
    * Setting member functions
    * --------------------------------------------------------------------------------
    */
    void setPosition(Vertex newPos);
    void setAngle(float newAngle);
    void setPosition(float x, float y);
    void setMovement(bool mouvement);
    void takeDamage(int amount);
    void setWeapon(Weapon* w);
    void setMaxHealth(int h) { maxHealth = h; }
    void addScore(int points) { score += points; }
    void resetScore() { score = 0; }
    void moveEnemy(const Actor& P, float deltaTime);
    void resetPlayerHealth();

    /*
    * --------------------------------------------------------------------------------
    * Status functions
    * --------------------------------------------------------------------------------
    */
    bool isMoving() const { return canMove; }

    //---Range Enemy Attack---
    void setRanged(bool r) { m_isRanged = r;}
    bool canShootProjectile();
    void triggerShootAnim(){m_isShooting = true; m_shootAnimTimer.restart();}
    bool isShooting()const {return m_isShooting;}
    float getShootAnimElapsed()const { return m_shootAnimTimer.elapsed() / 1000.0f;}
    void updateShootAnim();

};

#endif
