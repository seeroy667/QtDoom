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
#include <QTimer>

#include "geostructs.h"
#include "Actor.h"
#include "bsp.h"
#include "projectile.h"

class RenderManager
{

public:
    RenderManager(QGraphicsScene* scene, int screenWidth, int screenHeight);

    void renderWall(const Linedef& wall, const std::vector<Vertex>& verteces, const Actor& player, const std::vector<Sector>& sectors);

    void render(Actor m_player,
                const std::vector<Actor*>& enemies,
                const std::vector<Actor*>& rangedEnemies,
                const std::vector<Projectile>& projectiles,
                const std::vector<Vertex>& heals,
                BSP* bsp,
                const std::vector<Vertex>& verteces,
                const std::vector<Sector>& sectors);
    void renderActor(Actor* actor, const Actor player, QColor color, float sizeMultiplier = 1.0f, bool isRanged = false);
    std::vector<Linedef> getRenderedWalls();
    void renderRay(float targetScreenX, float targetScreenY, int frames);
    float projectHeight(float worldHeight, float distance);
    Vertex coordPlayer(const Vertex& point, const Actor& player);
    QGraphicsView* getView() const;
    void renderGun();
    void updateScreenSize(int width, int height);
    void setHit(bool h) {hit = h;}
    void setPowerUpActive(bool active) { m_isPowerUpActive = active; }
    void triggerGunAnim();
    void renderHeals(const std::vector<Vertex>& heals, const Actor& player);

private:
    QGraphicsScene* m_scene;
    int m_screenWidth;
    int m_screenHeight;
    float m_focalLength;
    float distanceMin;
    std::vector<Linedef> renderedWalls;
    int m_rayFramesLeft = 0;
    float m_rayTargetX = 0;
    float m_rayTargetY = 0;
    float m_rayStartX = 0;
    float m_rayStartY = 0;
    bool hit = false;
    QPixmap m_wallTexture;
    QPixmap m_enemyTexture;
    QPixmap m_gunTexture;
    bool m_isPowerUpActive = false;
    QPixmap m_gunFrames[3];
    int     m_gunFrame = 0;
    bool    m_gunAnimating = false;
    QElapsedTimer m_gunAnimTimer;
    float   m_frameDuration = 0.1f;
    QPixmap m_enemyFrames[4];
    int     m_enemyFrame = 0;
    bool    m_enemyAnimating = false;
    QElapsedTimer m_enemyAnimTimer;
    float   m_enemyFrameDuration = 0.15;
    QPixmap m_rangedEnemyTexture;
    QPixmap m_rangedShootFrames[3];


    bool clipWall(Vertex& p1, Vertex& p2);
    Vertex projectToScreen(const Vertex& cameraPoint);




};

#endif // RENDERMANAGER_H
