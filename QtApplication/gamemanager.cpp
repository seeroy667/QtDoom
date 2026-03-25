#include "gamemanager.h"

GameManager::GameManager() {
    p = new Actor();
    p->setPosition(0.0f, -2.0f);
    p->setAngle(0.0f);

    //temporaire a modifier
    e = new Actor();
    e->setPosition(5.0f,5.0f);
    e->setAngle(0.0f);

    map = new MapReader();

    cManager = nullptr;
}

void GameManager::restartGame()
{
    p->setPosition(0.0f, 0.0f);
    p->setAngle(0.0f);
    p->getWeapon()->resetGameAmmo();
    p->resetPlayerHealth();
    updateVie();
    //faut aussi replacer les monstres
}

Actor* GameManager::getPlayer()
{
    return p;
}

Actor* GameManager::getEnemy()
{
    return e;
}

void GameManager::loadMap(const std::string& filename)
{
    if (!map->load(filename))
    {
        qDebug() << "ERROR: Failed to load map";
        return;
    }

    verteces = map->getVerteces();
    linedefs = map->getLinedefs();
    sectors = map->getSectors();

    bsp = new BSP();

    m_playerWeapon = new Weapon(1, 1000.0f, 50.0f, 10, 2.5f);
    p->setWeapon(m_playerWeapon);

    bsp->build(linedefs, verteces);
    cManager = new CollisionManager();

     m_spawnPoints = bsp->collectValidSpawnPoints(verteces, 5.0f);
}

BSP* GameManager::getBSP()
{
    return bsp;
}

void GameManager::spawnWave(int count)
{
    if(m_spawnPoints.empty())
    {
        qDebug() << "Aucun spawn point sur la map";
        return;
    }

    std::vector<Vertex> shuffled = m_spawnPoints;
    for(int i = (int)shuffled.size() - 1; i > 0; i--)
    {
        int j = rand() % (i+1);
        std::swap(shuffled[i], shuffled[j]);
    }

    int spawned = 0;
    for(const Vertex& pos : shuffled)
    {
        if(spawned >= count) break;

        float dx = pos.x - p->getPosition().x;
        float dy = pos.y - p->getPosition().y;
        if(std::sqrt(dx*dx + dy*dy) < 5.0f) continue;

        Actor* enemy = new Actor();
        enemy->setPosition(pos.x,pos.y);
        enemy->setAngle(0.0f);
        creatures.push_back(enemy);
        spawned++;
    }

}

bool GameManager::isWaveClear() const
{
    for(Actor* e : creatures)
        if(e->getHealth() > 0) return false;
    return true;
}

void GameManager::update(float deltaTime, std::vector<Linedef> renderedWalls)
{
    // --- Vagues ---
    if (!m_waveActive)
    {
        m_currentWave++;
        if (m_currentWave <= (int)m_waveSizes.size())
        {
            for (Actor* a : creatures) delete a;
            creatures.clear();
            spawnWave(m_waveSizes[m_currentWave - 1]);
            m_waveActive = true;
        }
    }
    else if (isWaveClear())
    {
        m_waveActive = false;
    }

    // --- Collision joueur ---
    std::vector<Linedef> broadedWalls;
    bsp->actorToWallBroading(p->getPosition(), broadedWalls, verteces);
    cManager->narrowingToCollide(broadedWalls, verteces, p);

    // --- Mise à jour creatures---
    for (Actor* enemy : creatures)
    {
        if (enemy->getHealth() <= 0) continue;

        enemy->setMovement(true);  
        enemy->moveEnemy(*p, deltaTime);

        // Collision ennemi avec les murs
        std::vector<Linedef> enemyWalls;
        bsp->actorToWallBroading(enemy->getPosition(), enemyWalls, verteces);
        cManager->narrowingToCollide(enemyWalls, verteces, enemy);

        // Dégâts au joueur
        if (inRadius(p, enemy))
        {
            if (!m_inContact)
            {
                m_inContact = true;
                m_enemyAttackTimer.restart();
                p->takeDamage(1);
                updateVie();
            }
            else if (m_enemyAttackTimer.elapsed() >= m_attackCooldown)
            {
                m_inContact = false;
                p->takeDamage(1);
                updateVie();
                m_enemyAttackTimer.restart();
            }

            if (p->getHealth() < 1)
                emit playerDead();
        }
    }
}



bool GameManager::inRadius(Actor* p, Actor* e)
{
    float radius = 4.0f;
    float dx = p->getPosition().x - e->getPosition().x;
    float dy = p->getPosition().y - e->getPosition().y;

    float distance = (dx * dx) + (dy * dy);
    if (distance < (radius * radius)) return true;
    return false;
}

bool GameManager::shoot(QPoint mousePos, QSize screenSize)
{

    Weapon* weapon = p->getWeapon();
    if (!weapon) return false;


    if (!weapon->canShoot())
    {
        qDebug() << "Cooldown pas écoulé";
        return false;
    }

    weapon->shoot();

    float screenW    = screenSize.width();
    float focalLength = screenW / 2.0f;


    float camDirX = (mousePos.x() - screenW / 2.0f) / focalLength;
    float camDirY = 1.0f;
    float len     = std::sqrt(camDirX * camDirX + camDirY * camDirY);
    camDirX /= len;
    camDirY /= len;

    // Convertit la direction caméra en direction monde
    float playerAngle = p->getAngle();
    float cosA        = std::cos(playerAngle);
    float sinA        = std::sin(playerAngle);
    float worldDirX = camDirX * cosA - camDirY * sinA;
    float worldDirY = camDirX * sinA + camDirY * cosA;

    Vertex playerPos  = p->getPosition();


    float maxDistance = weapon->getRange();
    float step        = 0.05f;

    for (float d = 0; d < maxDistance; d += step)
    {
        float rayX = playerPos.x + worldDirX * d;
        float rayY = playerPos.y + worldDirY * d;

        float dx = rayX - e->getPosition().x;
        float dy = rayY - e->getPosition().y;

        if ((dx*dx + dy*dy) < (1.5f * 1.5f))
        {
            qDebug() << "Touché à distance:" << d;
            e->takeDamage(weapon->getDamage());
            return true;
        }
    }

    qDebug() << "Manqué";
    return false;
}

void GameManager::updateVie()
{
    int vie=p->getHealth();
    emit sigUpdateVie(vie);
}

Weapon* GameManager::getWeapon()
{
    return m_playerWeapon;
}


void GameManager::collectAllWalls(Node* node, std::vector<Linedef>& walls)
{
    if (!node) return;

    walls.push_back(node->partition);
    collectAllWalls(node->front, walls);
    collectAllWalls(node->back, walls);
}


std::vector<Actor*> GameManager::getRenderedEnemy()
{
    std::vector<Actor*> enemies;
    for (Actor* enemy : creatures)
    {
        if (bsp->enemyRendering(p->getPosition(), enemy->getPosition(), verteces))
            enemies.push_back(enemy);
    }

    return enemies;
}

