/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Player.h
Goal: Header file for the Player class managing player attributes and actions.
Modifications:
*/

#ifndef PLAYER_H
#define PLAYER_H

struct Vertices
{
    int x;
    int y;
};

class Player {
private:
    Vertices position;
    int health;
    bool isAlive;
    float angle;
public:
    Player();
    ~Player();
    void move(float deltaX, float deltaY);
    void shoot();
    void takeDamage(int amount);


    int getHealth();
    float getAngle() const;
    Vertices getPosition() const;

    void setAngle(float newAngle);
    void rotate(float deltaAngle);
    void setPosition(float x, float y);
};

#endif
