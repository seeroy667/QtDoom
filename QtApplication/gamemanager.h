/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Engine.h
Description: Header file for the game engine handling all events.
Modifications:
*/

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include<string>
#include <QApplication>
#include <QProcess>
#include <QElapsedTimer>
#include <QObject>
#include <QRandomGenerator>

#include"actor.h"
#include"geostructs.h"
#include"bsp.h"
#include "Weapon.h"
#include "mapreader.h"
#include "collisionmanager.h"
#include "projectile.h"

class GameManager: public QObject
{
    Q_OBJECT
public:
    GameManager();
    ~GameManager();
    Actor* getPlayer();
    Actor* getEnemy();
    BSP* getBSP();
    void loadMap(const std::string& filename);
    void update(float deltaTime, std::vector<Linedef> renderedWalls);
    bool inRadius(Actor *p, Actor *e);
    bool shoot(QPoint mousePos, QSize screenSize);
    const std::vector<Vertex>& getVerteces() const {return verteces;};
    const std::vector<Linedef>& getLinedefs() const {return linedefs;};
    const std::vector<Sector>& getSectors() const {return sectors;};
    void updateVie();
    Weapon* getWeapon();
    void restartGame();
    std::vector<Actor*> getRenderedEnemy();
    void collectAllWalls(Node* node, std::vector<Linedef>& walls);
    void spawnWave(int count);
    bool isWaveClear() const;
    int getCurrentWave() const {return m_currentWave;}
    std::vector<Actor*>& getCreatures(){return creatures;}
    Actor* getBoss();
    bool isBossRenderable();

    //Heal
    void spawnHealIfNeeded();
    const std::vector<Vertex>& getHeals() const { return m_healPickups;}
    void checkHealPickup();

    //Projectile
    const std::vector<Projectile>& getProjectiles() const {return m_projectiles;}
    const std::vector<Actor*>& getRangedEnemies() const { return m_rangedEnemies; }
    std::vector<Actor*> getRenderedRangedEnemies();
    const std::vector<Vertex>& getWeaponPickups() const { return m_weaponPickups; }
    bool playerHasShotgun() const { return m_playerHasShotgun; }


private:
    Actor *p;
    Actor *e;
    std::vector<Actor*> creatures;
    Actor* m_boss = nullptr;
    void SpawnBoss();
    bool m_bossAlive = false;
    bool m_bossSpawn = false;
    BSP* bsp = nullptr;
    Weapon* m_playerWeapon = nullptr;
    MapReader* map;
    CollisionManager* cManager;

    // Map data
    std::vector<Vertex> verteces;
    std::vector<Linedef> linedefs;
    std::vector<Sector> sectors;

    QElapsedTimer m_enemyAttackTimer;
    float m_attackCooldown = 1000.0f;
    bool m_inContact = false;

    std::vector<Vertex> m_spawnPoints;
    int waveSizeForWave(int wave) const { return 5 + wave; }
    int m_currentWave = 0;
    bool m_waveActive = false;

    std::vector<Vertex> m_healPickups;
    int m_lastHealScore = 0;

    std::vector<Actor*> m_rangedEnemies;
    std::vector<Projectile> m_projectiles;
    void spawnRangedWave(int count, const std::vector<Vertex>& usedPositions);
    void updateProjectiles(float deltaTime);
    std::vector<Vertex> m_weaponPickups;
    bool m_playerHasShotgun = false;
    void spawnWeaponPickup();
    void checkWeaponPickup();
    int m_shotgunWave = -1;

    int bestScore = 0;

signals:
    void sigUpdateVie(int value);
    void sigUpdateBalles(int value);
    void playerDead();
    void scoreResult(int value);
    void sigWeaponChanged();

public slots:
    void giveScore();
    void saveBestScore();
    void resetBestScore();
};

#endif // GAMEMANAGER_H
