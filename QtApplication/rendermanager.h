/*
Author: Mathieu Vincent
Date: Febuary 12, 2026
File name: Painter.h
Goal: Header file for the wall renderer
Modifications:
*/

#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QColor>

#include "geostructs.h"
#include "Actor.h"
#include "bsp.h"

class RenderManager
{

public:
    RenderManager(QGraphicsScene* scene, int screenWidth, int screenHeight);

    void renderWall(const Linedef& wall, const Actor& player);

    void render(Actor m_player, Actor m_enemy, BSP* bsp);
    void renderActor(const Actor m_actor, const Actor m_player, QColor color);

private:
    QGraphicsScene* m_scene;
    int m_screenWidth;
    int m_screenHeight;
    float m_focalLength;
    float distanceMin;

    Vertex coordPlayer(const Vertex& point, const Actor& player);

    bool clipWall(Vertex& p1, Vertex& p2);

    Vertex projectToScreen(const Vertex& cameraPoint);

    float projectHeight(float worldHeight, float distance);

};

#endif // RENDERMANAGER_H
