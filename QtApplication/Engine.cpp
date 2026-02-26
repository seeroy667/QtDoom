/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Engine.cpp
Description: Code file for the game engine handling all events.
Modifications:
*/

#include"engine.h"

Engine::Engine(QGraphicsScene *scene, QWidget* widget, int width, int height, QObject *parent)
    : QObject(parent)
{
    cManager = new ControllerManager();
    gManager = new GameManager();
    rManager = new RenderManager(scene, width, height);
    uiManager = new UIManager();

    m_width = width;
    m_height = height;
    m_widget = widget;

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
    if(cManager->justShot())
    {
        QPoint globalMousePos = QCursor::pos();
        QPoint localMousePos = m_widget->mapFromGlobal(globalMousePos);
        bool hit = gManager->shoot(localMousePos, QSize(m_width, m_height));


        float screenW = m_width;
        float focalLength = screenW / 2.0f;
        float camDirX = (localMousePos.x() - screenW / 2.0f) / focalLength;

        float startX = localMousePos.x();
        float startY =localMousePos.y();


        Vertex camEnemy = rManager->coordPlayer(gManager->getEnemy()->getPosition(),
                                                *gManager->getPlayer());
        float endX, endY;
        if (hit && camEnemy.y > 0.1f)
        {
            endX = (camEnemy.x / camEnemy.y) * focalLength + screenW / 2.0f;
            endY = rManager->projectHeight(2.5f, camEnemy.y);
        }
        else
        {
            // Prolonger dans la direction de la souris jusqu'au bord
            endX = startX + camDirX * 1000.0f;
            endY = startY; // rayon horizontal simplifié
        }

        rManager->renderRay(startX, startY, endX, endY, 60);
        cManager->resetShot();
    }

    rManager->render(*gManager->getPlayer(),*gManager->getEnemy(), gManager->getBSP());
    gManager->update(deltaTime, rManager->getRenderedWalls());

}

ControllerManager* Engine::getcManager() const
{
    return cManager;
}
