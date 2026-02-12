#include "WallRenderer.h"
#include <QColor>
#include <algorithm>
#include <cmath>


WallRenderer::WallRenderer(QGraphicsScene* scene, int screenWidth, int screenHeight)
{
    m_focalLength = screenWidth / 2.0f;
    distanceMin = 0.1f;

    m_scene = scene;
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
}


void WallRenderer::renderWall(const Wall& wall, const Player& player)
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



Point2D WallRenderer::coordPlayer(const Point2D& point, const Player& player)
{
    Vertices playerPos = player.getPosition();
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



bool WallRenderer::clipWall(Point2D& p1, Point2D& p2)
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


Point2D WallRenderer::projectToScreen(const Point2D& cameraPoint)
{
    Point2D screen;
    screen.x = (cameraPoint.x / cameraPoint.y) * m_focalLength
               + m_screenWidth / 2.0f;
    screen.y = 0;
    return screen;
}


float WallRenderer::projectHeight(float worldHeight, float distance)
{
    float eyeHeight = 2.5f;

    float relativeHeight = worldHeight - eyeHeight;

    float screenHeight = (relativeHeight / distance) * m_focalLength;

    return m_screenHeight / 2.0f - screenHeight;
}
