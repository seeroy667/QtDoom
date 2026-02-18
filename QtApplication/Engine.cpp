/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Engine.cpp
Description: Code file for the game engine handling all events.
Modifications:
*/

#include"engine.h"

Engine::Engine(QGraphicsScene *scene, int width, int height, QObject *parent)
    : QObject(parent)
{
    cManager = new ControllerManager();
    gManager = new GameManager();
    rManager = new RenderManager(scene, width, height);
    p = new Actor();
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
    //float deltaTime = elapsedTimer.restart() / 1000.0f;
    //if (deltaTime > 0.1f) deltaTime = 0.1f; Will be usefull when treating deltaTime (gap in between frames)

    if(cManager->movingBack()) p->setPosition(p->getPosition().x+(0.5f*cos(p->getAngle() - M_PI/2)), p->getPosition().y+(0.5f*sin(p->getAngle() - M_PI/2)));
    if(cManager->movingLeft()) p->setPosition(p->getPosition().x-(0.5f*cos(p->getAngle())), p->getPosition().y-(0.5f*sin(p->getAngle())));
    if(cManager->movingRight()) p->setPosition(p->getPosition().x+(0.5f*cos(p->getAngle())), p->getPosition().y+(0.5f*sin(p->getAngle())));
    if(cManager->movingFront()) p->setPosition(p->getPosition().x+(0.5f*cos(p->getAngle() + M_PI/2)), p->getPosition().y+(0.5f*sin(p->getAngle() + M_PI/2)));
    if(cManager->rotatingLeft()) p->setAngle(p->getAngle()+0.05f);
    if(cManager->rotatingRight()) p->setAngle(p->getAngle()-0.05f);

    rManager->render(*p);
}

ControllerManager* Engine::getcManager() const
{
    return cManager;
}
