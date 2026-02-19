/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Engine.h
Description: Header file for the game engine handling all events.
Modifications:
*/

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include"actor.h"

class GameManager
{
public:
    GameManager();
    Actor* getPlayer();
private:
    Actor *p;
};

#endif // GAMEMANAGER_H
