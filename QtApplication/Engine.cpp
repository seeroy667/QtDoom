/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Engine.cpp
Description: Code file for the game engine handling all events.
Modifications:
*/

#include"Engine.h"

Engine::Engine(QGraphicsScene *scene, int width, int height, QObject *parent)
    : QObject(parent)
{
    cManager = new Controller();
    gManager = new GameManager();
    rManager = new WallRenderer(scene, width, height);
    p = new Player();
    p->setPosition(0.0f, 0.0f);
    p->setAngle(0.0f);

    // This was missing — without it gameLoop() never gets called
    connect(&timer, &QTimer::timeout, this, &Engine::gameLoop);
}

Engine::~Engine()
{
}

void Engine::start()
{
    elapsedTimer.start();
    timer.start(1000 / TARGET_FPS);
}

void Engine::gameLoop()
{
    float deltaTime = elapsedTimer.restart() / 1000.0f;

    if (deltaTime > 0.1f) deltaTime = 0.1f;

    p->setAngle(p->getAngle()+0.05f);
    rManager->render(*p);
}
