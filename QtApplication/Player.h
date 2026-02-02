/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Player.h
Goal: Header file for the Player class managing player attributes and actions.
Modifications:
*/

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
public:
    Player();
    ~Player();
    void move(float deltaX, float deltaY);
    void shoot();
    void takeDamage(int amount);
    int getHealth();
};
