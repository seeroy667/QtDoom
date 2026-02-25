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
    uiManager = new UIManager();

    gManager->loadMap("WadLvl1.txt");
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
    if (deltaTime > 0.1f) deltaTime = 0.1f; //Will be usefull when treating deltaTime (gap in between frames)

    if(cManager->movingBack()) gManager->getPlayer()->setPosition(gManager->getPlayer()->getPosition().x+(0.5f*cos(gManager->getPlayer()->getAngle() - M_PI/2)), gManager->getPlayer()->getPosition().y+(0.5f*sin(gManager->getPlayer()->getAngle() - M_PI/2)));
    if(cManager->movingLeft()) gManager->getPlayer()->setPosition(gManager->getPlayer()->getPosition().x-(0.5f*cos(gManager->getPlayer()->getAngle())), gManager->getPlayer()->getPosition().y-(0.5f*sin(gManager->getPlayer()->getAngle())));
    if(cManager->movingRight()) gManager->getPlayer()->setPosition(gManager->getPlayer()->getPosition().x+(0.5f*cos(gManager->getPlayer()->getAngle())), gManager->getPlayer()->getPosition().y+(0.5f*sin(gManager->getPlayer()->getAngle())));
    if(cManager->movingFront()) gManager->getPlayer()->setPosition(gManager->getPlayer()->getPosition().x+(0.5f*cos(gManager->getPlayer()->getAngle() + M_PI/2)), gManager->getPlayer()->getPosition().y+(0.5f*sin(gManager->getPlayer()->getAngle() + M_PI/2)));
    if(cManager->rotatingLeft()) gManager->getPlayer()->setAngle(gManager->getPlayer()->getAngle()+0.05f);
    if(cManager->rotatingRight()) gManager->getPlayer()->setAngle(gManager->getPlayer()->getAngle()-0.05f);
    if(cManager->isShooting())
    {
        QPoint mousePos = QCursor::pos();
        gManager->shoot(mousePos);
    }

    rManager->render(*gManager->getPlayer(),*gManager->getEnemy(), gManager->getBSP());
    gManager->update(deltaTime, rManager->getRenderedWalls());
}

ControllerManager* Engine::getcManager() const
{
    return cManager;
}
