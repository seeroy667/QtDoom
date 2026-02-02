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
