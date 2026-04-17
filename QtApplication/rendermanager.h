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

struct PolygonCoordinates
{
    // For the clipping, the column position (index of columns) where it starts and where it ends
    int columnStart;
    int columnEnd;

    // Screen positions of the polygon
    float topRight;
    float topLeft;
    float botRight;
    float botLeft;
};
struct ColumnInfo {
    int topPosition;
    int bottomPosition;
};

class RenderManager
{

public:
    /*
     * --------------------------------------------------------------------------------
     * Constructer functions
     * --------------------------------------------------------------------------------
     */
    RenderManager(QGraphicsScene* scene, int screenWidth, int screenHeight);
    void setTextures();

    /*
     * --------------------------------------------------------------------------------
     * Rendering logic
     * --------------------------------------------------------------------------------
     */
    void render(Actor m_player,
                const std::vector<Actor*>& enemies,
                const std::vector<Actor*>& rangedEnemies,
                const std::vector<Projectile>& projectiles,
                const std::vector<Vertex>& heals,
                const std::vector<Vertex>& weaponPickups,
                BSP* bsp,
                const std::vector<Vertex>& verteces,
                const std::vector<Sector>& sectors);
    void renderWall(const Linedef& wall,
                    const std::vector<Vertex>& verteces,
                    const Actor& player,
                    const std::vector<Sector>& sectors);
    void renderActor(Actor* actor, const Actor player, QColor color, float sizeMultiplier = 1.0f, bool isRanged = false);

    void renderGun();

    void renderHeals(const std::vector<Vertex>& heals, const Actor& player);

    void renderWeaponPickups(const std::vector<Vertex>& pickups, const Actor& player);

    /*
     * --------------------------------------------------------------------------------
     * Utilities
     * --------------------------------------------------------------------------------
     */
    float projectHeight(float worldHeight, float distance);

    Vertex coordPlayer(const Vertex& point, const Actor& player);

    QGraphicsView* getView() const;

    void updateScreenSize(int width, int height);

    void setHit(bool h) {hit = h;}

    void setPowerUpActive(bool active) { m_isPowerUpActive = active; }

    void triggerGunAnim();

    void setShotgunMode(bool hasShotgun);
    void renderFloorAndCeiling();

private:
    // Screen
    QGraphicsScene* m_scene;
    int m_screenWidth;
    int m_screenHeight;
    float m_focalLength;
    float distanceMin;

    // Walls
    std::vector<Linedef> renderedWalls;
    int m_rayFramesLeft = 0;

    // Rays
    float m_rayTargetX = 0;
    float m_rayTargetY = 0;
    float m_rayStartX = 0;
    float m_rayStartY = 0;

    // Guns
    int     m_gunFrame = 0;
    QElapsedTimer m_gunAnimTimer;
    float   m_frameDuration = 0.12f;

    // Enemies
    int     m_enemyFrame = 0;
    QElapsedTimer m_enemyAnimTimer;
    float   m_enemyFrameDuration = 0.15;

    // States
    bool m_hasShotgun = false;
    bool    m_enemyAnimating = false;
    bool    m_gunAnimating = false;
    bool hit = false;
    bool m_isPowerUpActive = false;

    // Textures
    QPixmap m_shotgunMapTexture;
    QPixmap m_shotgunIdleTexture;
    QPixmap m_shotgunFrames[4];
    QPixmap m_rangedEnemyTexture;
    QPixmap m_rangedShootFrames[3];
    QPixmap m_enemyFrames[4];
    QPixmap m_gunFrames[3];
    QPixmap m_wallTexture;
    QPixmap m_enemyTexture;
    QPixmap m_gunTexture;

    // For occlusion
    int m_closedColumns;
    std::vector<float> columnDepths;
    std::vector<float> spriteDepths;
    std::vector<ColumnInfo> columns;


    bool clipWall(Vertex& p1, Vertex& p2);
    Vertex projectToScreen(const Vertex& cameraPoint);
};

#endif // RENDERMANAGER_H

