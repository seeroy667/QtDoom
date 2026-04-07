/*
Author: Mathieu Vincent
Date: Febuary 12, 2026
File name: Painter.cpp
Goal: Code for rendering walls.
Modifications:
    Date: March 24, 2026
        Author: Donavan Sirois
        Description: Added FOV culling and overdraw manager. For a map of 152 walls, 87 were rendered after clipping.
        After these integrations, for the same map, we render walls.
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

    m_wallTexture = QPixmap(":/ressources/temp.jpg");
    m_enemyTexture = QPixmap(":/ressources/enemy.jpg");
    m_gunTexture = QPixmap(":/ressources/arme.png");
    m_gunFrames[0] = QPixmap(":/ressources/shoot1.png");
    m_gunFrames[1] = QPixmap(":/ressources/shoot2.png");
    m_gunFrames[2] = QPixmap(":/ressources/shoot3.png");
}


void RenderManager::renderWall(const Linedef& wall, const std::vector<Vertex>& verteces, const Actor& player, const std::vector<Sector>& sectors)
{
    Vertex p1 = coordPlayer(verteces[wall.start], player);
    Vertex p2 = coordPlayer(verteces[wall.end], player);

    if (!clipWall(p1, p2))
        return;

    // Need to handle FOV culling before rendering the walls.
    // Need to handle overdraw to only render a small amount of walls.

    Vertex screen1 = projectToScreen(p1);
    Vertex screen2 = projectToScreen(p2);

    float height1_floor = projectHeight(sectors[wall.sideFront].floorHeight, p1.y);
    float height1_ceil  = projectHeight(sectors[wall.sideFront].ceilingHeight, p1.y);

    float height2_floor = projectHeight(sectors[wall.sideFront].floorHeight, p2.y);
    float height2_ceil  = projectHeight(sectors[wall.sideFront].ceilingHeight, p2.y);

    QPolygonF polygon;
    polygon << QPointF(screen1.x, height1_ceil)
            << QPointF(screen2.x, height2_ceil)
            << QPointF(screen2.x, height2_floor)
            << QPointF(screen1.x, height1_floor);

    QGraphicsPolygonItem* wallItem = m_scene->addPolygon(polygon);

    float wallWidth  = std::abs(screen2.x - screen1.x);
    float wallHeight = std::max(std::abs(height1_floor - height1_ceil),
                                std::abs(height2_floor - height2_ceil));

    if (wallWidth < 1.0f)  wallWidth  = 1.0f;
    if (wallHeight < 1.0f) wallHeight = 1.0f;

    // Scale via transformation sans créer un nouveau QPixmap
    float scaleX = wallWidth  / m_wallTexture.width();
    float scaleY = wallHeight / m_wallTexture.height();

    QTransform transform;
    transform.translate(std::min(screen1.x, screen2.x),
                        std::min(height1_ceil, height2_ceil));
    transform.scale(scaleX, scaleY);

    QBrush textureBrush(m_wallTexture);
    textureBrush.setTransform(transform);

    wallItem->setBrush(textureBrush);
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


void RenderManager::renderActor(Actor* actor, const Actor player, QColor color, float sizeMultiplier)
{

    if (actor->getHealth() <= 0) return;

    Vertex camPos = coordPlayer(actor->getPosition(), player);
    if (camPos.y < distanceMin) return;

    float screenX = (camPos.x / camPos.y) * m_focalLength + m_screenWidth / 2.0f;


    float spriteBottom = projectHeight(0.0f, camPos.y);
    float spriteTop    = projectHeight(5.0f, camPos.y);
    float baseSize     = spriteBottom - spriteTop;


    float squareSize = baseSize * sizeMultiplier;
    float spriteTopAdjusted = spriteBottom - squareSize;

    QRectF spriteRect(
        screenX - squareSize / 2.0f,
        spriteTopAdjusted,
        squareSize,
        squareSize
        );

    if (spriteRect.right() < 0 || spriteRect.left() > m_screenWidth)
        return;


    QGraphicsRectItem* spriteItem = m_scene->addRect(spriteRect);
    spriteItem->setPen(Qt::NoPen);
    if (!m_enemyTexture.isNull())
    {
        float scaleX = squareSize / m_enemyTexture.width();
        float scaleY = squareSize / m_enemyTexture.height();

        QTransform transform;
        transform.translate(spriteRect.left(), spriteRect.top());
        transform.scale(scaleX, scaleY);

        QBrush textureBrush(m_enemyTexture);
        textureBrush.setTransform(transform);

        spriteItem->setBrush(textureBrush);
    }
    else
    {
        int brightness = std::max(0, std::min(255,
                                              (int)(255.0f / (1.0f + camPos.y / 10.0f))));
        QColor shadedColor(
            color.red()   * brightness / 255,
            color.green() * brightness / 255,
            color.blue()  * brightness / 255
            );
        spriteItem->setBrush(shadedColor);
    }
}

void RenderManager::renderRay(float targetScreenX, float targetScreenY, int frames)
{
    m_rayFramesLeft = frames;
    m_rayTargetX = targetScreenX;
    m_rayTargetY = targetScreenY;

}

void RenderManager::renderGun()
{
    if (m_gunAnimating)
    {
        int frameIndex = (int)(m_gunAnimTimer.elapsed() / 1000.0f / m_frameDuration);
        if (frameIndex >= 3)
        {
            frameIndex = 0;
        }
        m_gunFrame = frameIndex;
    }

    int gunWidth  = 500;
    int gunHeight = 250;
    float gunX = (m_screenWidth  / 2.0f) - (gunWidth  / 2.0f);
    float gunY = (m_screenHeight - gunHeight);

    QPixmap scaled = m_gunFrames[m_gunFrame].scaled(gunWidth, gunHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem *gunItem = m_scene->addPixmap(scaled);
    gunItem->setPos(gunX, gunY);
}

void RenderManager::triggerGunAnim()
{
    m_gunFrame = 0;
    m_gunAnimating = true;
    m_gunAnimTimer.restart();
}

void RenderManager::render(Actor m_player,
                           const std::vector<Actor*>& enemies,
                           BSP* bsp,
                           const std::vector<Vertex>& verteces,
                           const std::vector<Sector>& sectors)
{
    m_scene->clear();
    bsp->traverse(m_player.getPosition(), renderedWalls, verteces);

    for (const Linedef& wall : renderedWalls) {
        renderWall(wall, verteces, m_player, sectors);
    }

    qDebug() << enemies.size();
    for (Actor* enemy : enemies)
       renderActor(enemy, m_player, QColor(255, 0, 0));

    float gunX = (m_screenWidth / 2.0f) - (200 / 2.0f);
    float gunY = (m_screenHeight - 100);
    if (m_rayFramesLeft > 0)
    {
        if(hit)
        {
            m_scene->addEllipse(m_rayTargetX, m_rayTargetY, 25, 25, QPen(QColor(255, 255, 0), 3),QBrush(QColor(255, 255, 0, 255)));
        }
        m_scene->addEllipse(gunX+90,gunY-20,25, 25, QPen(QColor(255, 255, 0), 3),QBrush(QColor(255, 255, 0, 255)));
        m_rayFramesLeft--;
    }
    renderGun();

    if(m_isPowerUpActive)
    {
        QGraphicsRectItem* overlay = m_scene->addRect(0, 0, m_screenWidth, m_screenHeight);
        overlay->setBrush(QColor(255, 0, 0, 60)); // 60 = transparence
        overlay->setPen(Qt::NoPen);
    }
}

void RenderManager::updateScreenSize(int width, int height)
{
    m_screenWidth = width;
    m_screenHeight = height;
    m_focalLength = width/2.0f;
}


std::vector<Linedef> RenderManager::getRenderedWalls()
{
    return renderedWalls;
}

QGraphicsView* RenderManager::getView() const
{
    if (m_scene && !m_scene->views().isEmpty())
        return m_scene->views().first();
    return nullptr;
}
