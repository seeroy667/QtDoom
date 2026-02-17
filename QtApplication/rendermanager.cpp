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


void RenderManager::renderWall(const Wall& wall, const Actor& player)
{
    Point2D p1 = coordPlayer(wall.start, player);
    Point2D p2 = coordPlayer(wall.end, player);

    if (!clipWall(p1, p2))
        return;

    Point2D screen1 = projectToScreen(p1);
    Point2D screen2 = projectToScreen(p2);

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



Point2D RenderManager::coordPlayer(const Point2D& point, const Actor& player)
{
    Point2D playerPos = player.getPosition();
    float anglePlayer = player.getAngle();

    float dx = point.x - playerPos.x;
    float dy = point.y - playerPos.y;

    float cosAngle = std::cos(-anglePlayer);
    float sinAngle = std::sin(-anglePlayer);

    Point2D camera;
    camera.x = dx * cosAngle - dy * sinAngle;
    camera.y = dx * sinAngle + dy * cosAngle;

    return camera;
}



bool RenderManager::clipWall(Point2D& p1, Point2D& p2)
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


Point2D RenderManager::projectToScreen(const Point2D& cameraPoint)
{
    Point2D screen;
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

// UTILISEE POUR EXEMPLE
void RenderManager::render(Actor m_player)
{
    m_scene->clear();

    std::vector<Wall> walls;

    // Mur du milieu
    Wall wall1;
    wall1.start = {-5.0f, 10.0f};
    wall1.end = {5.0f, 10.0f};
    wall1.floorHeight = 0.0f;
    wall1.ceilingHeight = 5.0f;
    walls.push_back(wall1);

    // Mur de gauche
    Wall wall2;
    wall2.start = {-5.0f, 10.0f};
    wall2.end = {-5.0f, 5.0f};
    wall2.floorHeight = 0.0f;
    wall2.ceilingHeight = 5.0f;
    walls.push_back(wall2);

    // Mur de droite
    Wall wall3;
    wall3.start = {5.0f, 5.0f};
    wall3.end = {5.0f, 10.0f};
    wall3.floorHeight = 0.0f;
    wall3.ceilingHeight = 5.0f;
    walls.push_back(wall3);

    for (const Wall& wall : walls) {
        renderWall(wall, m_player);
    }
}
