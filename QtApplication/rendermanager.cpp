/*
Author: Mathieu Vincent
Date: Febuary 12, 2026
File name: Painter.cpp
Goal: Code for rendering walls.
Modifications:
*/


#include "rendermanager.h"
#include <QColor>
#include <algorithm>
#include <cmath>


RenderManager::RenderManager(QGraphicsScene* scene, int screenWidth, int screenHeight)
{
    m_focalLength = screenWidth / 2.0f;
    distanceMin = 0.1f;

    m_scene = scene;
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
}


void RenderManager::renderWall(const Linedef& wall, const Actor& player)
{
    Vertex p1 = coordPlayer(wall.start, player);
    Vertex p2 = coordPlayer(wall.end, player);

    if (!clipWall(p1, p2))
        return;

    Vertex screen1 = projectToScreen(p1);
    Vertex screen2 = projectToScreen(p2);

    float height1_floor = projectHeight(wall.floorHeight, p1.y);
    float height1_ceil  = projectHeight(wall.ceilingHeight, p1.y);

    float height2_floor = projectHeight(wall.floorHeight, p2.y);
    float height2_ceil  = projectHeight(wall.ceilingHeight, p2.y);

    QPolygonF polygon;
    polygon << QPointF(screen1.x, height1_ceil)
            << QPointF(screen2.x, height2_ceil)
            << QPointF(screen2.x, height2_floor)
            << QPointF(screen1.x, height1_floor);

    QGraphicsPolygonItem* wallItem = m_scene->addPolygon(polygon);

    float avgDistance = (p1.y + p2.y) / 2.0f;
    int brightness = std::max(0, std::min(255,
                                          (int)(255.0f / (1.0f + avgDistance / 10.0f))));

    wallItem->setBrush(QColor(brightness, brightness, brightness));
    wallItem->setPen(Qt::NoPen);
}



Vertex RenderManager::coordPlayer(const Vertex& point, const Actor& player)
{
    Vertex playerPos = player.getPosition();
    float anglePlayer = player.getAngle();

    float dx = point.x - playerPos.x;
    float dy = point.y - playerPos.y;

    float cosAngle = std::cos(-anglePlayer);
    float sinAngle = std::sin(-anglePlayer);

    Vertex camera;
    camera.x = dx * cosAngle - dy * sinAngle;
    camera.y = dx * sinAngle + dy * cosAngle;

    return camera;
}



bool RenderManager::clipWall(Vertex& p1, Vertex& p2)
{
    if (p1.y < distanceMin && p2.y < distanceMin)
        return false;

    if (p1.y < distanceMin)
    {
        float t = (distanceMin - p1.y) / (p2.y - p1.y);
        p1.x = p1.x + t * (p2.x - p1.x);
        p1.y = distanceMin;
    }

    if (p2.y < distanceMin)
    {
        float t = (distanceMin - p2.y) / (p1.y - p2.y);
        p2.x = p2.x + t * (p1.x - p2.x);
        p2.y = distanceMin;
    }

    return true;
}


Vertex RenderManager::projectToScreen(const Vertex& cameraPoint)
{
    Vertex screen;
    screen.x = (cameraPoint.x / cameraPoint.y) * m_focalLength
               + m_screenWidth / 2.0f;
    screen.y = 0;
    return screen;
}


float RenderManager::projectHeight(float worldHeight, float distance)
{
    float eyeHeight = 2.5f;

    float relativeHeight = worldHeight - eyeHeight;

    float screenHeight = (relativeHeight / distance) * m_focalLength;

    return m_screenHeight / 2.0f - screenHeight;
}


void RenderManager::renderActor(const Actor actor, const Actor player, QColor color)
{

    Vertex camPos = coordPlayer(actor.getPosition(), player);


    if (camPos.y < distanceMin)
        return;


    float screenX = (camPos.x / camPos.y) * m_focalLength + m_screenWidth / 2.0f;


    float spriteHeight = (1.0f / camPos.y) * m_focalLength;
    float spriteWidth  = spriteHeight * 2.0f;



    float actorHeight = 3.0f;
    float eyeHeight   = 2.5f;
    float spriteBottom = projectHeight(0.0f, camPos.y);
    float spriteTop    = projectHeight(actorHeight, camPos.y);
    float squareSize   = spriteBottom - spriteTop;

    QRectF spriteRect(
        screenX - squareSize / 2.0f,
        spriteTop,
        squareSize,
        squareSize
        );

    if (spriteRect.right() < 0 || spriteRect.left() > m_screenWidth)
        return;


    int brightness = std::max(0, std::min(255,
                                          (int)(255.0f / (1.0f + camPos.y / 10.0f))));
    QColor shadedColor(
        color.red()   * brightness / 255,
        color.green() * brightness / 255,
        color.blue()  * brightness / 255
        );


    QGraphicsRectItem* spriteItem = m_scene->addRect(spriteRect);
    spriteItem->setBrush(shadedColor);
    spriteItem->setPen(Qt::NoPen);
}

// UTILISEE POUR EXEMPLE
void RenderManager::render(Actor m_player, Actor m_enemy)
{
    m_scene->clear();

    std::vector<Linedef> walls;

    // Mur du milieu
    Linedef wall1;
    wall1.start = {-5.0f, 10.0f};
    wall1.end = {5.0f, 10.0f};
    wall1.floorHeight = 0.0f;
    wall1.ceilingHeight = 5.0f;
    walls.push_back(wall1);

    // Mur de gauche
    Linedef wall2;
    wall2.start = {-5.0f, 10.0f};
    wall2.end = {-5.0f, 5.0f};
    wall2.floorHeight = 0.0f;
    wall2.ceilingHeight = 5.0f;
    walls.push_back(wall2);

    // Mur de droite
    Linedef wall3;
    wall3.start = {5.0f, 5.0f};
    wall3.end = {5.0f, 10.0f};
    wall3.floorHeight = 0.0f;
    wall3.ceilingHeight = 5.0f;
    walls.push_back(wall3);

    for (const Linedef& wall : walls) {
        renderWall(wall, m_player);
    }
    renderActor(m_enemy, m_player, QColor(255,0,0));
}
