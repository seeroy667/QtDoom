/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Player.cpp
Goal: Code for the Player class managing player attributes and actions.
Modifications:
*/

#include "Player.h"


Player::Player()
{
    position = {0, 0};
    angle = 0.0f;
    health = 100;
    isAlive = true;
}

Player::~Player()
{

}

void Player::move(float deltaX, float deltaY)
{
    position.x += deltaX;
    position.y += deltaY;
}

void Player::shoot()
{
    // Shooting logic here
}

void Player::takeDamage(int amount)
{
    health -= amount;
    if (health < 0)
    {
        health = 0;
        isAlive = false;
    }
}

int Player::getHealth()
{
    return health;
}

float Player::getAngle() const
{
    return angle;
}

Vertices Player::getPosition() const
{
    return position;
}

void Player::setAngle(float a)
{
    angle = a;
}

void Player::rotate(float a)
{
    angle += a;
}

void Player::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;
}


