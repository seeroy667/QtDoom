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
    m_enemyTexture = QPixmap(":/ressources/Demon5.png");
    m_gunTexture = QPixmap(":/ressources/arme.png");
    m_gunFrames[0] = QPixmap(":/ressources/shoot1.png");
    m_gunFrames[1] = QPixmap(":/ressources/shoot2.png");
    m_gunFrames[2] = QPixmap(":/ressources/shoot3.png");
    m_enemyFrames[0] = QPixmap(":/ressources/Demon1.png");
    m_enemyFrames[1] = QPixmap(":/ressources/Demon2.png");
    m_enemyFrames[2] = QPixmap(":/ressources/Demon3.png");
    m_enemyFrames[3] = QPixmap(":/ressources/Demon4.png");
    m_rangedEnemyTexture = QPixmap(":/ressources/range1.png");
    m_rangedShootFrames[0] = QPixmap(":/ressources/range1.png");
    m_rangedShootFrames[1] = QPixmap(":/ressources/range2.png");
    m_rangedShootFrames[2] = QPixmap(":/ressources/range3.png");
    m_enemyAnimTimer.start();

}


void RenderManager::renderWall(const Linedef& wall, const std::vector<Vertex>& verteces, const Actor& player, const std::vector<Sector>& sectors)
{
    Vertex p1 = coordPlayer(verteces[wall.start], player);
    Vertex p2 = coordPlayer(verteces[wall.end], player);

    if (!clipWall(p1, p2))
        return;

    float depth = (p1.y + p2.y) / 2.0f;

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
    wallItem->setZValue(-depth);

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


void RenderManager::renderActor(Actor* actor, const Actor player, QColor color, float sizeMultiplier, bool isRanged)
{
    if (actor->getHealth() <= 0) return;

    Vertex camPos = coordPlayer(actor->getPosition(), player);

    if (camPos.y < distanceMin)
        return;


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


    QPixmap currentTexture;
    if(isRanged)
    {
        if(actor->isShooting())
        {
            int frameIndex = (int)(actor->getShootAnimElapsed()/0.15f);
            frameIndex = std::min(frameIndex,2);
            if(!m_rangedShootFrames[frameIndex].isNull())
                currentTexture = m_rangedShootFrames[frameIndex];
            else
                currentTexture = m_rangedEnemyTexture;
        }
        else
        {
            currentTexture = m_rangedEnemyTexture;
        }

    }
    else
    {
        if (actor->isMoving())
        {
            // Animation de déplacement boucle sur les 4 frames
            int frameIndex = (int)(m_enemyAnimTimer.elapsed() / 1000.0f / m_enemyFrameDuration) % 4;
            if (!m_enemyFrames[frameIndex].isNull())
                currentTexture = m_enemyFrames[frameIndex];
            else
                currentTexture = m_enemyTexture;
        }
        else
        {
            currentTexture = m_enemyTexture;
        }
    }
    QGraphicsRectItem* spriteItem = m_scene->addRect(spriteRect);
    spriteItem->setZValue(-camPos.y);
    spriteItem->setPen(Qt::NoPen);

    if (!currentTexture.isNull())
    {
        float scaleX = squareSize / currentTexture.width();
        float scaleY = squareSize / currentTexture.height();

        QTransform transform;
        transform.translate(spriteRect.left(), spriteRect.top());
        transform.scale(scaleX, scaleY);

        QBrush textureBrush(currentTexture);
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
                           const std::vector<Actor*>& rangedEnemies,
                           const std::vector<Projectile>& projectiles,
                           const std::vector<Vertex>& heals,
                           BSP* bsp,
                           const std::vector<Vertex>& verteces,
                           const std::vector<Sector>& sectors)
{
    m_scene->clear();
    bsp->traverse(m_player.getPosition(), renderedWalls, verteces);

    for (const Linedef& wall : renderedWalls) {
        renderWall(wall, verteces, m_player, sectors);
    }

    //ennemis melee
    for (Actor* enemy : enemies)
       renderActor(enemy, m_player, QColor(255, 0, 0), 1.0f, false);
    //ennemis distance
    for(Actor* enemy : rangedEnemies)
        renderActor(enemy, m_player, QColor(200,100,0), 1.0f, true);

    //Projectiles
    for (const Projectile& proj : projectiles)
    {
        Vertex camPos = coordPlayer(proj.position, m_player);
        if (camPos.y < distanceMin) continue;

        // Angle relatif entre la caméra et le projectile
        float angleToProj = std::atan2(camPos.x, camPos.y);

        // FOV de 90 degrés (pi/2), donc on mappe l'angle sur la largeur de l'écran
        float halfFov = M_PI / 2.0f;
        if (std::abs(angleToProj) > halfFov) continue; // hors FOV

        // Projection basée sur l'angle plutôt que sur x/y directement
        float screenX = (angleToProj / halfFov) * (m_screenWidth / 2.0f) + m_screenWidth / 2.0f;

        float distance = std::sqrt(camPos.x * camPos.x + camPos.y * camPos.y);
        float screenY = projectHeight(2.5f, camPos.y);

        float size = (m_focalLength / camPos.y) * 2.5f;
        size = std::max(25.0f, std::min(size, 80.0f));

        m_scene->addEllipse(screenX - size/2, screenY - size/2, size, size,
                            QPen(QColor(180, 0, 255), 3),
                            QBrush(QColor(100, 0, 255, 180)));

        float haloSize = size * 1.4f;
        m_scene->addEllipse(screenX - haloSize/2, screenY - haloSize/2, haloSize, haloSize,
                            QPen(QColor(200, 100, 255, 120), 2),
                            QBrush(Qt::NoBrush));
    }
    //heal
    renderHeals(heals, m_player);

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

void RenderManager::renderHeals(const std::vector<Vertex>& heals, const Actor& player)
{
    for(const Vertex& heal : heals)
    {
        Vertex camPos = coordPlayer(heal,player);
        if(camPos.y < distanceMin) return;

        float screenX = (camPos.x / camPos.y) * m_focalLength + m_screenWidth / 2.0f;
        float spriteBottom = projectHeight(0.0f, camPos.y);
        float spriteTop    = projectHeight(5.0f, camPos.y);
        float size = (spriteBottom - spriteTop) * 0.6f;

        if (screenX + size < 0 || screenX - size > m_screenWidth) continue;

        float centerY = (spriteBottom + spriteTop) / 2.0f;
        float thickness = size * 0.25f;

        // Barre horizontale
        QGraphicsRectItem* hBar = m_scene->addRect(
            screenX - size / 2.0f,
            centerY - thickness / 2.0f,
            size,
            thickness
            );
        hBar->setBrush(QColor(0, 200, 0));
        hBar->setPen(Qt::NoPen);

        // Barre verticale
        QGraphicsRectItem* vBar = m_scene->addRect(
            screenX - thickness / 2.0f,
            centerY - size / 2.0f,
            thickness,
            size
            );
        vBar->setBrush(QColor(0, 200, 0));
        vBar->setPen(Qt::NoPen);
    }
}
