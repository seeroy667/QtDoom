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

#include "geostructs.h"
#include "actor.h"

class RenderManager
{
public:
    RenderManager(QGraphicsScene* scene, int screenWidth, int screenHeight);

    void renderWall(const Wall& wall, const Actor& player);

    void render(Actor m_player);

private:
    QGraphicsScene* m_scene;
    int m_screenWidth;
    int m_screenHeight;
    float m_focalLength;
    float distanceMin;

    Point2D coordPlayer(const Point2D& point, const Actor& player);

    bool clipWall(Point2D& p1, Point2D& p2);

    Point2D projectToScreen(const Point2D& cameraPoint);

    float projectHeight(float worldHeight, float distance);
};

#endif // RENDERMANAGER_H
