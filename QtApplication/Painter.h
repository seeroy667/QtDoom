/*
Author: Mathieu Vincent
Date: Febuary 12, 2026
File name: Painter.h
Goal: Header file for the wall renderer
Modifications:
*/

#ifndef PAINTER_H
#define PAINTER_H

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPolygonItem>
#include <QPolygonF>

#include "geoStructs.h"
#include "Player.h"

class WallRenderer
{
public:
    WallRenderer(QGraphicsScene* scene, int screenWidth, int screenHeight);

    void renderWall(const Wall& wall, const Player& player);

    void render(Player m_player);

private:
    QGraphicsScene* m_scene;
    int m_screenWidth;
    int m_screenHeight;
    float m_focalLength;
    float distanceMin;

    Point2D coordPlayer(const Point2D& point, const Player& player);

    bool clipWall(Point2D& p1, Point2D& p2);

    Point2D projectToScreen(const Point2D& cameraPoint);

    float projectHeight(float worldHeight, float distance);
};

#endif // PAINTER_H
