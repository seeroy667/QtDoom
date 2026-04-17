/*
Author: Mathieu Vincent
Date: February 1, 2026
File name: Player.cpp
Goal: Code for the Player class managing player attributes and actions.
Modifications:
*/

#include "actor.h"
#include <cmath>

Actor::Actor()
{
    // Initialize base attributes for player/enemy
    position = {0, 0};
    angle = 0.0f;
    health = 5;
    maxHealth = 5;
    isAlive = true;
}

Actor::~Actor()
{

}

// Move the actor by a given offset
void Actor::move(float deltaX, float deltaY)
{
    position.x += deltaX;
    position.y += deltaY;
}

// Apply damage to the actor
void Actor::takeDamage(int amount)
{
    health -= amount;

    // If health drops below 0 → actor dies
    if (health < 0)
    {
        health = 0;
        isAlive = false;
    }
}

// Set the actor's angle directly
void Actor::setAngle(float a)
{
    angle = a;
}

// Rotate the actor by adding to its current angle
void Actor::rotate(float a)
{
    angle += a;
}

// Set position using x and y
void Actor::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;
}

// --------------------------------------------------
// Enemy-related functions
// --------------------------------------------------

// Compute distance between Enemy (E) and Player (P)
float Actor::distancePlayerEnemy(const Actor& E, const Actor& P)
{
    dx = P.getPosition().x - E.getPosition().x;
    dy = P.getPosition().y - E.getPosition().y;

    return std::sqrt(dx * dx + dy * dy);
}

// Move enemy toward the player
void Actor::moveEnemy(const Actor& Player, float deltaTime)
{
    // If movement is disabled, do nothing
    if (!canMove) return;

    // Direction vector from enemy to player
    float distX = Player.getPosition().x - position.x;
    float distY = Player.getPosition().y - position.y;

    float distance = std::sqrt(distX * distX + distY * distY);

    // Update enemy angle to face the player
    angle = std::atan2(distY, distX);

    // Move only if not too close
    if (distance > 1.0f)
    {
        // Normalize direction vector
        float nx = distX / distance;
        float ny = distY / distance;

        // Move toward player using speed and deltaTime
        position.x += nx * EnemySpeed * deltaTime;
        position.y += ny * EnemySpeed * deltaTime;
    }
}

// --------------------------------------------------
// Weapon handling
// --------------------------------------------------

// Assign a weapon to the actor
void Actor::setWeapon(Weapon *w)
{
    m_weapon = w;
}

// Get the actor's weapon
Weapon* Actor::getWeapon() const
{
    return m_weapon;
}

// Check if actor has a weapon
bool Actor::hasWeapon() const
{
    return m_weapon != nullptr;
}

// Enable or disable movement
void Actor::setMovement(bool movement)
{
    canMove = movement;
}

// Reset player health to default values
void Actor::resetPlayerHealth()
{
    health = 5;
    maxHealth = 5;
}

// Set position using a Vertex struct
void Actor::setPosition(Vertex newPos)
{
    position = newPos;
}

// --------------------------------------------------
// Shooting logic (cooldown-based)
// --------------------------------------------------

bool Actor::canShootProjectile()
{
    // First shot starts the timer
    if (!m_timerStarted)
    {
        m_shootTimer.start();
        m_timerStarted = true;
        return false;
    }

    // Check if cooldown has passed
    if (m_shootTimer.elapsed() >= m_shootCooldown)
    {
        m_shootTimer.restart();
        return true;
    }

    return false;
}

// Update shooting animation state
void Actor::updateShootAnim()
{
    // Stop shooting animation after duration * 3
    if (m_isShooting && m_shootAnimTimer.elapsed() / 1000.0f > m_shootAnimDuration * 3)
        m_isShooting = false;
}
