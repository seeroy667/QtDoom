/*
Author: Mathieu Vincent
Date: Febuary 12, 2026
File name: Painter.cpp
Goal: Code for rendering walls.
Modifications:
    Date: April 13, 2026
        Author: Donavan Sirois
        Description: Added overdraw manager. We now render front to back.
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
    qDebug() << "Screenwidth" << screenWidth;
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
    m_shotgunMapTexture  = QPixmap(":/ressources/shotgunMap.png");
    m_shotgunIdleTexture = QPixmap(":/ressources/shotgun.png");
    m_shotgunFrames[0]   = QPixmap(":/ressources/tir1.png");
    m_shotgunFrames[1]   = QPixmap(":/ressources/tir2.png");
    m_shotgunFrames[2]   = QPixmap(":/ressources/tir3.png");
    m_shotgunFrames[3]   = QPixmap(":/ressources/tir4.png");
    m_enemyAnimTimer.start();

    // For occlusion
    columns.resize(screenWidth);
}

void RenderManager::render(Actor m_player,
                           const std::vector<Actor*>& enemies,
                           const std::vector<Actor*>& rangedEnemies,
                           const std::vector<Projectile>& projectiles,
                           const std::vector<Vertex>& heals,
                           const std::vector<Vertex>& weaponPickups,
                           BSP* bsp,
                           const std::vector<Vertex>& verteces,
                           const std::vector<Sector>& sectors)
{

    // Initializing the columns on screen for clipping
    m_scene->clear();
    columns.resize(m_screenWidth);
    for (int i = 0; i < m_screenWidth; i++)
    {
        columns[i].topPosition = 0;
        columns[i].bottomPosition = m_screenHeight;
    }
    m_closedColumns = 0;
    renderedWalls.clear();


    // This is an anonymous function callback within a method in a different class
    // Trying to find a solution to stop a function in its track under a certain
    // condition when in another class led me (Donavan Sirois) to learning this
    // Pretty interesting thing to anyone who wants to read on it
    // You basically define a small scoped function you pass in parameter which computes the condition you want.
    // When the condition is met, the method of the other class returns
    // Here, when all columns are full on screen, we stop parsing through the BSP.
    bsp->traverseAndRender(
        bsp->getRoot(),
        m_player.getPosition(),
        verteces,
        [&](const Linedef& wall) -> bool // template of the anonymous function: [capture] (parameters) -> return value { code };
        {
            // in this case, we capture everything as adresses, the paremeters
            //passed is the wall, the return value is a boolean, and then the code is below
            renderWall(wall, verteces, m_player, sectors);
            renderedWalls.push_back(wall);
            return m_closedColumns < m_screenWidth; // Check if all columns are full
        });

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
        if (camPos.y < 0.5f) continue;

        float screenX = (camPos.x / camPos.y) * m_focalLength + m_screenWidth / 2.0f;
        float screenY = projectHeight(2.5f, camPos.y);


        if (screenX < -m_screenWidth || screenX > m_screenWidth * 2) continue;

        float size = (m_focalLength / camPos.y) * 0.8f;
        size = std::max(8.0f, std::min(size, 120.0f));

        if (screenX + size < 0 || screenX - size > m_screenWidth) continue;

        QGraphicsEllipseItem* ball = m_scene->addEllipse(
            screenX - size / 2.0f,
            screenY - size / 2.0f,
            size, size,
            QPen(QColor(180, 0, 255), 3),
            QBrush(QColor(100, 0, 255, 180)));
        ball->setZValue(9999.0f);

        // Halo extérieur
        float haloSize = size * 2.0f;
        QGraphicsEllipseItem* halo = m_scene->addEllipse(
            screenX - haloSize / 2.0f,
            screenY - haloSize / 2.0f,
            haloSize, haloSize,
            QPen(QColor(200, 100, 255, 120), 2),
            QBrush(Qt::NoBrush));
        halo->setZValue(9998.0f);
    }
    //heal
    renderHeals(heals, m_player);

    //Shotgun
    renderWeaponPickups(weaponPickups, m_player);

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

void RenderManager::renderWall(const Linedef& wall, const std::vector<Vertex>& verteces, const Actor& player, const std::vector<Sector>& sectors)
{
    Vertex p1 = coordPlayer(verteces[wall.start], player);
    Vertex p2 = coordPlayer(verteces[wall.end], player);

    if (!clipWall(p1, p2)) return; // The wall is behind the player, and does not need to be rendered

    // Perspective calculations
    Vertex screen1 = projectToScreen(p1);
    Vertex screen2 = projectToScreen(p2);

    if (screen1.x > screen2.x)
    {
        std::swap(p1, p2);
        std::swap(screen1, screen2);
    }

    float polyLength = screen2.x - screen1.x;
    if (polyLength == 0) return;

    // Culling verification. The rendering now works with the following logic:
    // We parse through the entire length of the wall. For each column, we check if it is full.
    // If it isn't, we parse through adding new polygons to render as we go if the wall is split multiple times.
    std::vector<PolygonCoordinates> polygonsToRender;
    PolygonCoordinates currentPolygon;
    bool isInVector = false;

    if (std::isnan(screen1.x) || std::isnan(screen2.x))
        return;

    int x1 = std::max(0, (int)std::ceil(screen1.x));
    int x2 = std::min(m_screenWidth - 1, (int)std::floor(screen2.x));

    if (x1 > x2) return;

    for (int i = x1; i <= x2; i++)
    {
        if (columns[i].topPosition >= columns[i].bottomPosition) // Column is full
        {
            if (isInVector) // If a polygon was being built on the last iteration (last column wasn't full) we add it to be rendered
            {
                polygonsToRender.push_back(currentPolygon);
                isInVector = false;
            }
            continue;
        }

        // We need to compute where in the wall the current column is
        // (i.e. if it was in the middle of the polygon (the rest was clipped), we need the coordinates of the reduced polygon)
        float t = (i - screen1.x) / polyLength;


        // Linear interpolation. We want the depth (Distance between the camera of the player to the new point of the wall)
        float invZ =  (1.0f / p1.y) + t * ((1.0f / p2.y) - (1.0f / p1.y));
        float depth = 1.0f / invZ;

        // New wall coordinates computation
        float wallTop = projectHeight(sectors[wall.sideFront].ceilingHeight,  depth);
        float wallBottom = projectHeight(sectors[wall.sideFront].floorHeight, depth);

        float drawTop = std::max(wallTop, (float)columns[i].topPosition);
        float drawBot = std::min(wallBottom, (float)columns[i].bottomPosition);

        // Start a new polygon if needed (Last column saw itself add a polygon)
        if (!isInVector)
        {
            currentPolygon.columnStart = i;
            currentPolygon.topLeft = drawTop;
            currentPolygon.botLeft = drawBot;
            isInVector = true;
        }

        // Extend the polygon to the new column
        currentPolygon.columnEnd = i;
        currentPolygon.topRight = drawTop;
        currentPolygon.botRight = drawBot;

        // Update the new drawing heights for the current column, so that the next parse knows how much of the column is used
        columns[i].topPosition = (int)drawTop;
        columns[i].bottomPosition = (int)drawBot;
    }

    if (isInVector) polygonsToRender.push_back(currentPolygon); // Added if was building a polygon and reached the end of the screen
    // End of overdraw and culling computing

    // Drawing all of the polygons
    for (const PolygonCoordinates& polygon : polygonsToRender)
    {
        QPolygonF p;
        p << QPointF(polygon.columnStart, polygon.topLeft)
          << QPointF(polygon.columnEnd, polygon.topRight)
          << QPointF(polygon.columnEnd, polygon.botRight)
          << QPointF(polygon.columnStart, polygon.botLeft);

        QGraphicsPolygonItem* wallItem = m_scene->addPolygon(p);
        wallItem->setBrush(QColor(60, 60, 60));
    }
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

    // Computing depth to render order properly
    float dx = player.getPosition().x - actor->getPosition().x;
    float dy = player.getPosition().y - actor->getPosition().y;
    float depth = sqrt(dx*dx + dy*dy);

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
    float healthPercent = (float)actor->getHealth() / (float)actor->getMaxHealth();

    float barWidth  = squareSize;
    float barHeight = 6.0f;
    float barX      = screenX - squareSize / 2.0f;
    float barY      = spriteTopAdjusted - 10.0f; // au dessus du sprite

    // Fond rouge
    QGraphicsRectItem* barBg = m_scene->addRect(barX, barY, barWidth, barHeight);
    barBg->setBrush(QColor(150, 0, 0));
    barBg->setPen(Qt::NoPen);
    barBg->setZValue(-depth + 1000.1f);


    QGraphicsRectItem* barFg = m_scene->addRect(barX, barY, barWidth * healthPercent, barHeight);
    barFg->setBrush(QColor(0, 200, 0));
    barFg->setPen(Qt::NoPen);
    barFg->setZValue(-depth + 1000.2f);

    QGraphicsRectItem* spriteItem = m_scene->addRect(spriteRect);
    spriteItem->setZValue(camPos.y);
    spriteItem->setPen(Qt::NoPen);
    spriteItem->setZValue(-depth + 1000.0f);

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
    int maxFrames = m_hasShotgun ? 4 : 3;

    if (m_gunAnimating)
    {
        int frameIndex = (int)(m_gunAnimTimer.elapsed() / 1000.0f / m_frameDuration);
        if (frameIndex >= maxFrames)
        {
            m_gunAnimating = false;
            m_gunFrame = 0;
        }
        else
        {
            m_gunFrame = frameIndex;
        }
    }

    int gunWidth  = 500;
    int gunHeight = 250;
    float gunX = (m_screenWidth / 2.0f) - (gunWidth / 2.0f);
    float gunY = (m_screenHeight - gunHeight);

    QPixmap toRender;
    if (m_gunAnimating)
        toRender = m_hasShotgun ? m_shotgunFrames[m_gunFrame] : m_gunFrames[m_gunFrame];
    else
        toRender = m_hasShotgun ? m_shotgunIdleTexture : m_gunFrames[0];

    QPixmap scaled = toRender.scaled(gunWidth, gunHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem* gunItem = m_scene->addPixmap(scaled);
    gunItem->setPos(gunX, gunY);
}

void RenderManager::triggerGunAnim()
{
    m_gunFrame = 0;
    m_gunAnimating = true;
    m_gunAnimTimer.restart();
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
    for (const Vertex& heal : heals)
    {
        Vertex camPos = coordPlayer(heal, player);
        if (camPos.y < distanceMin) continue;

        float screenX = (camPos.x / camPos.y) * m_focalLength + m_screenWidth / 2.0f;
        float spriteBottom = projectHeight(0.0f, camPos.y);
        float spriteTop    = projectHeight(5.0f, camPos.y);
        float size = (spriteBottom - spriteTop) * 0.6f;

        if (screenX + size < 0 || screenX - size > m_screenWidth) continue;

        float centerY  = (spriteBottom + spriteTop) / 2.0f;
        float thickness = size * 0.25f;

        QGraphicsRectItem* hBar = m_scene->addRect(
            screenX - size / 2.0f,
            centerY - thickness / 2.0f,
            size, thickness);
        hBar->setBrush(QColor(0, 200, 0));
        hBar->setPen(Qt::NoPen);
        hBar->setZValue(camPos.y);

        QGraphicsRectItem* vBar = m_scene->addRect(
            screenX - thickness / 2.0f,
            centerY - size / 2.0f,
            thickness, size);
        vBar->setBrush(QColor(0, 200, 0));
        vBar->setPen(Qt::NoPen);
        vBar->setZValue(camPos.y);
    }
}
void RenderManager::setShotgunMode(bool hasShotgun)
{
    m_hasShotgun = hasShotgun;
}

void RenderManager::renderWeaponPickups(const std::vector<Vertex>& pickups, const Actor& player)
{
    for (const Vertex& pickup : pickups)
    {
        Vertex camPos = coordPlayer(pickup, player);
        if (camPos.y < distanceMin) continue;

        float screenX = (camPos.x / camPos.y) * m_focalLength + m_screenWidth / 2.0f;
        float spriteBottom = projectHeight(0.0f, camPos.y);
        float spriteTop    = projectHeight(5.0f, camPos.y);
        float baseSize = (spriteBottom - spriteTop) * 0.8f;

        if (screenX + baseSize < 0 || screenX - baseSize > m_screenWidth) continue;

        float centerY = (spriteBottom + spriteTop) / 2.0f;


        float ratio  = m_shotgunMapTexture.isNull() ? 3.0f
                                                   : (float)m_shotgunMapTexture.width() / m_shotgunMapTexture.height();
        float height = baseSize;
        float width  = baseSize * ratio;

        QGraphicsRectItem* item = m_scene->addRect(
            screenX - width / 2.0f,
            centerY - height / 2.0f,
            width,
            height
            );
        item->setPen(Qt::NoPen);
        item->setZValue(camPos.y);

        if (!m_shotgunMapTexture.isNull())
        {
            float scaleX = width  / m_shotgunMapTexture.width();
            float scaleY = height / m_shotgunMapTexture.height();
            QTransform transform;
            transform.translate(screenX - width / 2.0f, centerY - height / 2.0f);
            transform.scale(scaleX, scaleY);
            QBrush brush(m_shotgunMapTexture);
            brush.setTransform(transform);
            item->setBrush(brush);
        }
        else
        {
            item->setBrush(QColor(200, 150, 50));
        }
    }
}
