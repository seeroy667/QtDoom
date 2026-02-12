#ifndef WALLRENDERER_H
#define WALLRENDERER_H

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <cmath>

#include "Player.h"

struct Point2D
{
    float x, y;
};

struct Wall
{
    Point2D start;
    Point2D end;
    float floorHeight;
    float ceilingHeight;
};


class WallRenderer
{
public:
    WallRenderer(QGraphicsScene* scene, int screenWidth, int screenHeight);

    void renderWall(const Wall& wall, const Player& player);

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

#endif // WALLRENDERER_H
