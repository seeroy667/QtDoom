/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Player.cpp
Goal: Code for the Player class managing player attributes and actions.
Modifications:
*/

#include "actor.h"
#include <cmath>

Actor::Actor()
{
    position = {0, 0};
    angle = 0.0f;
    health = 5;
    maxHealth = 5;
    isAlive = true;
}

/*
* --------------------------------------------------------------------------------
* Setting member functions
* --------------------------------------------------------------------------------
*/

void Actor::takeDamage(int amount)
{
    health -= amount;
    if (health < 0)
    {
        health = 0;
        isAlive = false;

    }
}

void Actor::setAngle(float a)
{
    angle = a;
}

void Actor::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;
}

void Actor::setMovement(bool movement)
{
    canMove = movement;
}

void Actor::resetPlayerHealth()
{
    health = 5;
    maxHealth = 5;
}

void Actor::setPosition(Vertex newPos)
{
    position = newPos;
}

//Enemy pathfinding
void Actor::moveEnemy(const Actor& Player, float deltaTime)
{
    if (!canMove) return;

    float distX    = Player.getPosition().x - position.x;
    float distY    = Player.getPosition().y - position.y;
    float distance = std::sqrt(distX*distX + distY*distY);

    angle = std::atan2(distY, distX);


    if (distance > 1.0f)
    {
        float nx = distX / distance;
        float ny = distY / distance;
        position.x += nx * EnemySpeed * deltaTime;
        position.y += ny * EnemySpeed * deltaTime;
    }
}

void Actor::setWeapon(Weapon *w)
{
    m_weapon = w;
}

/*
* --------------------------------------------------------------------------------
* Status functions
* --------------------------------------------------------------------------------
*/

bool Actor::canShootProjectile()
{
    if(!m_timerStarted)
    {
        m_shootTimer.start();
        m_timerStarted = true;
        return false;
    }
    if(m_shootTimer.elapsed() >= m_shootCooldown)
    {
        m_shootTimer.restart();
        return true;
    }
    return false;
}

void Actor::updateShootAnim()
{
    if(m_isShooting && m_shootAnimTimer.elapsed() /1000.0f > m_shootAnimDuration * 3)
        m_isShooting = false;
}

