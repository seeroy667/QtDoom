/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Engine.h
Description: Header file for the game engine handling all events.
Modifications:
*/

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include<string>
#include <QApplication>
#include <QProcess>

#include"actor.h"
#include"geostructs.h"
#include"bsp.h"

class GameManager
{
public:
    GameManager();
    Actor* getPlayer();
    Actor* getEnemy();
    BSP* getBSP();
    void loadMap(const std::string& filename);
    void update(float deltaTime, std::vector<Linedef> renderedWalls);
    bool inRadius(Actor *p, Actor *e);
private:
    Actor *p;
    Actor *e;
    std::vector<Actor*> creatures;
    std::vector<Linedef> walls;
    BSP* bsp;
};

#endif // GAMEMANAGER_H
