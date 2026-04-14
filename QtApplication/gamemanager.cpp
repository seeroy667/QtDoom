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
    p->resetScore();
    updateVie();
    m_healPickups.clear();
    m_lastHealScore = 0;
    m_currentWave = 0;
    m_waveActive = false;
    for (Actor* a : m_rangedEnemies) delete a;
    m_rangedEnemies.clear();
    m_projectiles.clear();
    for (Actor* a : creatures) delete a;
    creatures.clear();
    if (m_boss)
    {
        delete m_boss;
        m_boss = nullptr;
    }
    m_bossAlive = false;
    m_bossSpawn = false;
    m_weaponPickups.clear();
    m_playerHasShotgun = false;
    m_shotgunWave = -1;

    delete m_playerWeapon;
    m_playerWeapon = new Weapon(1, 1000.0f, 10.0f, 10, 2.0f);
    p->setWeapon(m_playerWeapon);
    emit sigWeaponChanged();
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

    m_playerWeapon = new Weapon(1, 1000.0f, 10.0f, 10, 2.0f);
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
    std::vector<Vertex> usedPositions;

    for(const Vertex& pos : shuffled)
    {
        if(spawned >= count) break;

        float dx = pos.x - p->getPosition().x;
        float dy = pos.y - p->getPosition().y;
        if(std::sqrt(dx*dx + dy*dy) < 5.0f) continue;

        Actor* enemy = new Actor();
        enemy->setPosition(pos.x, pos.y);
        enemy->setAngle(0.0f);
        creatures.push_back(enemy);
        usedPositions.push_back(pos);
        spawned++;
    }


    if(spawned < count && !usedPositions.empty())
    {
        int i = 0;
        while(spawned < count)
        {
            Vertex pos = usedPositions[i % usedPositions.size()];
            Actor* enemy = new Actor();
            enemy->setPosition(pos.x, pos.y);
            enemy->setAngle(0.0f);
            creatures.push_back(enemy);
            spawned++;
            i++;
        }
    }
    for (Actor* a : m_rangedEnemies) delete a;
    m_rangedEnemies.clear();
    m_projectiles.clear();

    if (m_currentWave == 2 || m_currentWave == 5 || m_currentWave == 9 && !m_playerHasShotgun)
        spawnWeaponPickup();

    spawnRangedWave(1 + m_currentWave, usedPositions);

}

bool GameManager::isWaveClear() const
{
    for(Actor* e : creatures)
        if(e->getHealth() > 0) return false;
    for (Actor* e : m_rangedEnemies)
        if (e->getHealth() > 0) return false;
    return true;
}

void GameManager::update(float deltaTime, std::vector<Linedef> renderedWalls)
{
    // --- Vagues ---
    if (!m_waveActive)
    {
        // Boss toutes les 3 vagues
        if (m_currentWave % 3 == 0 && m_currentWave > 0 && !m_bossSpawn)
        {
            SpawnBoss();
            m_waveActive = true;
        }
        else
        {
            m_currentWave++;
            for (Actor* a : creatures) delete a;
            creatures.clear();
            m_bossSpawn = false;
            spawnWave(waveSizeForWave(m_currentWave));
            m_waveActive = true;
        }
    }

    else if (isWaveClear())
    {
        if(m_bossSpawn && m_boss && m_boss->getHealth() <= 0)
        {
            m_bossAlive = false;
            m_waveActive = false;
            qDebug() << "BOSS MORT";
        }
        else if(!m_bossSpawn)
        {
            if (m_playerHasShotgun && m_currentWave > m_shotgunWave)
            {
                m_playerHasShotgun = false;
                m_shotgunWave = -1;
                delete m_playerWeapon;
                m_playerWeapon = new Weapon(1, 1000.0f, 10.0f, 10, 2.0f); // retour arme de base
                p->setWeapon(m_playerWeapon);
                emit sigWeaponChanged();
            }
            m_waveActive = false;
        }
    }
    //---Spawn de Vie---
    checkHealPickup();

    //---ShotGun---
    checkWeaponPickup();

    //---mise a jour creature distance---
    updateProjectiles(deltaTime);

    for (Actor* enemy : m_rangedEnemies)
    {
        if (enemy->getHealth() <= 0) continue;
        enemy->updateShootAnim();

        Vertex ePos = enemy->getPosition();
        Vertex pPos = p->getPosition();
        float dx = pPos.x - ePos.x;
        float dy = pPos.y - ePos.y;
        float dist = std::sqrt(dx*dx + dy*dy);

        if (dist > 10.0f)
        {
            enemy->setMovement(true);
            enemy->moveEnemy(*p, deltaTime);
        }
        else
        {
            enemy->setMovement(false);
        }

        std::vector<Linedef> enemyWalls;
        bsp->actorToWallBroading(enemy->getPosition(), enemyWalls, verteces);
        cManager->narrowingToCollide(enemyWalls, verteces, enemy);
    }

    // --- Collision joueur ---
    std::vector<Linedef> broadedWalls;
    bsp->actorToWallBroading(p->getPosition(), broadedWalls, verteces);
    cManager->narrowingToCollide(broadedWalls, verteces, p);

    // --- Mise à jour creatures melee---
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
    if(m_bossAlive && m_boss && m_boss->getHealth() > 0)
    {
        m_boss->moveEnemy(*p,deltaTime);

        std::vector<Linedef> bossWalls;
        bsp->actorToWallBroading(m_boss->getPosition(), bossWalls, verteces);
        cManager->narrowingToCollide(bossWalls,verteces, m_boss);

        if(inRadius(p,m_boss))
        {
            p->takeDamage(2);
            updateVie();
            if(p->getHealth() < 1)
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

    if (weapon->isEmpty())
    {
        weapon->reload();
    }

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

        for (Actor* enemy : creatures)
        {
            if (enemy->getHealth() <= 0) continue;

            float dx = rayX - enemy->getPosition().x;
            float dy = rayY - enemy->getPosition().y;

            if ((dx*dx + dy*dy) < (1.5f * 1.5f))
            {
                qDebug() << "Touché à distance:" << d;
                enemy->takeDamage(weapon->getDamage());

                if (enemy->getHealth() <= 0)
                {
                    p->addScore(1);
                    spawnHealIfNeeded();
                }
                return true;
            }
        }

        for (Actor* enemy : m_rangedEnemies)
        {
            if (enemy->getHealth() <= 0) continue;

            float dx = rayX - enemy->getPosition().x;
            float dy = rayY - enemy->getPosition().y;

            if ((dx*dx + dy*dy) < (1.5f * 1.5f))
            {
                enemy->takeDamage(weapon->getDamage());
                if (enemy->getHealth() <= 0)
                    p->addScore(1);
                return true;
            }
        }

        if(m_bossAlive && m_boss && m_boss->getHealth() > 0)
        {
            float dx = rayX - m_boss->getPosition().x;
            float dy = rayY - m_boss->getPosition().y;

            if((dx*dx + dy*dy) < (2.0f * 2.0f))
            {
                m_boss->takeDamage(weapon->getDamage());
                if(m_boss->getHealth() <=0)
                {
                    m_bossAlive = false;
                    p->addScore(10);
                }
                return true;
            }
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
        if (enemy == m_boss) continue; // déjà là
        if (bsp->enemyRendering(p->getPosition(), enemy->getPosition(), verteces))
            enemies.push_back(enemy);
    }

    return enemies;
}


void GameManager::SpawnBoss()
{
    if(m_spawnPoints.empty())
    {
        qDebug() << "Aucun spawn point pour le boss";
        return;
    }
    Vertex bossSpawn = m_spawnPoints[0];
    for(const Vertex& pos : m_spawnPoints)
    {
        float dx = pos.x - p->getPosition().x;
        float dy = pos.y - p->getPosition().y;
        if(std::sqrt(dx*dx + dy*dy) > 8.0f)
        {
            bossSpawn = pos;
            break;
        }
    }
    m_boss = new Actor();
    m_boss->setHealth(20);
    m_boss->setMaxHealth(20);
    m_boss->setAngle(0.0f);
    m_boss->setPosition(bossSpawn.x, bossSpawn.y);
    m_bossAlive = true;
    m_bossSpawn = true;
}
Actor* GameManager::getBoss()
{
    return m_boss;
}

bool GameManager::isBossRenderable()
{
    return m_bossAlive && m_boss && m_boss->getHealth() > 0;
}

void GameManager::spawnHealIfNeeded()
{
    int score = p->getScore();
    if(score > 0 && score/5 > m_lastHealScore/5)
    {
        m_lastHealScore = score;
        int index = rand() % m_spawnPoints.size();
        m_healPickups.push_back(m_spawnPoints[index]);
    }
}
void GameManager::checkHealPickup()
{
    Vertex playerPos = p->getPosition();
    float pickUpRadius = 2.0f;

    for(int i = (int)m_healPickups.size() - 1; i>=0; i--)
    {
        float dx = playerPos.x - m_healPickups[i].x;
        float dy = playerPos.y - m_healPickups[i].y;
        if((dx*dx + dy*dy) < (pickUpRadius * pickUpRadius))
        {
            p->setHealth(p->getHealth()+1);
            updateVie();
            m_healPickups.erase(m_healPickups.begin() + i);
        }
    }
}

void GameManager::spawnRangedWave(int count, const std::vector<Vertex>& usedPositions)
{
    if (m_spawnPoints.empty()) return;

    std::vector<Vertex> shuffled = m_spawnPoints;
    for (int i = (int)shuffled.size()-1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        std::swap(shuffled[i], shuffled[j]);
    }

    int spawned = 0;
    for (const Vertex& pos : shuffled)
    {
        if (spawned >= count) break;


        float dx = pos.x - p->getPosition().x;
        float dy = pos.y - p->getPosition().y;
        if (std::sqrt(dx*dx + dy*dy) < 5.0f) continue;

        // Vérifier que la position n'est pas déjà utilisée par un ennemi mêlée
        bool alreadyUsed = false;
        for (const Vertex& used : usedPositions)
        {
            float ux = pos.x - used.x;
            float uy = pos.y - used.y;
            if ((ux*ux + uy*uy) < 1.0f)
            {
                alreadyUsed = true;
                break;
            }
        }
        if (alreadyUsed) continue;

        Actor* enemy = new Actor();
        enemy->setPosition(pos.x, pos.y);
        enemy->setAngle(0.0f);
        enemy->setRanged(true);
        m_rangedEnemies.push_back(enemy);
        spawned++;
    }
}

void GameManager::updateProjectiles(float deltaTime)
{
    for(Projectile& proj : m_projectiles)
    {
        if(!proj.active)continue;

        float speed = 15.0f;
        proj.position.x += proj.dirX * speed * deltaTime;
        proj.position.y += proj.dirY * speed * deltaTime;
        proj.distanceTraveled += speed * deltaTime;

        if(proj.distanceTraveled >= proj.maxDistance)
        {
            proj.active = false;
            continue;
        }

        //collision avec le joueur

        float dx = proj.position.x - p->getPosition().x;
        float dy = proj.position.y - p->getPosition().y;
        if((dx*dx + dy*dy) < (1.0f * 1.0f))
        {
            p->takeDamage(1);
            updateVie();
            proj.active = false;
            if(p->getHealth() < 1)
                emit playerDead();
        }
    }
    m_projectiles.erase(std::remove_if(m_projectiles.begin(), m_projectiles.end(),[](const Projectile& p) { return !p.active;}), m_projectiles.end());

    for(Actor* enemy : m_rangedEnemies)
    {
        if(enemy->getHealth() <= 0) continue;

        float dx = p->getPosition().x - enemy->getPosition().x;
        float dy = p->getPosition().y - enemy->getPosition().y;
        float dist = std::sqrt(dx*dx + dy*dy);

        if (dist > 30.0f) continue;

        if (enemy->canShootProjectile())
        {
            enemy->triggerShootAnim();
            Vertex ePos = enemy->getPosition();
            Vertex pPos = p->getPosition();

            float dx = pPos.x - ePos.x;
            float dy = pPos.y - ePos.y;
            float len = std::sqrt(dx*dx + dy*dy);
            if (len < 0.001f) continue;

            float dirX = dx / len;
            float dirY = dy / len;

            float spawnOffset = 2.0f;

            Projectile proj;
            proj.position.x = ePos.x + dirX * spawnOffset;
            proj.position.y = ePos.y + dirY * spawnOffset;
            proj.dirX = dirX;
            proj.dirY = dirY;
            m_projectiles.push_back(proj);
        }
    }
}
std::vector<Actor*> GameManager::getRenderedRangedEnemies()
{
    std::vector<Actor*> result;
    for (Actor* enemy : m_rangedEnemies)
    {
        if (enemy->getHealth() <= 0) continue;
        if (bsp->enemyRendering(p->getPosition(), enemy->getPosition(), verteces))
            result.push_back(enemy);
    }
    return result;
}


void GameManager::giveScore()
{
    emit scoreResult(bestScore);
}

GameManager::~GameManager()
{
    delete p;
    delete e;
    delete map;
    delete bsp;
    delete m_playerWeapon;
    delete cManager;

    if (m_boss)
    {
        delete m_boss;
    }

    for (Actor* a : m_rangedEnemies) delete a;
    m_rangedEnemies.clear();
    m_projectiles.clear();


    for (Actor* a : creatures) delete a;
    creatures.clear();
}

void GameManager::spawnWeaponPickup()
{
    if (m_spawnPoints.empty()) return;
    // Spawn à un point aléatoire loin du joueur
    for (const Vertex& pos : m_spawnPoints)
    {
        float dx = pos.x - p->getPosition().x;
        float dy = pos.y - p->getPosition().y;
        if (std::sqrt(dx*dx + dy*dy) > 8.0f)
        {
            m_weaponPickups.push_back(pos);
            break;
        }
    }
}

void GameManager::checkWeaponPickup()
{
    if (m_weaponPickups.empty()) return;
    Vertex playerPos = p->getPosition();
    float pickupRadius = 3.0f;

    for (int i = (int)m_weaponPickups.size() - 1; i >= 0; i--)
    {
        float dx = playerPos.x - m_weaponPickups[i].x;
        float dy = playerPos.y - m_weaponPickups[i].y;
        if ((dx*dx + dy*dy) < (pickupRadius * pickupRadius))
        {
            delete m_playerWeapon;
            m_playerWeapon = new Weapon(5, 1000.0f, 2.0f, 10, 2.0f);
            p->setWeapon(m_playerWeapon);
            m_playerHasShotgun = true;
            m_shotgunWave = m_currentWave;
            m_weaponPickups.erase(m_weaponPickups.begin() + i);
            emit sigWeaponChanged();
        }
    }
}

void GameManager::saveBestScore()
{
    int nowScore=p->getScore();
    if (nowScore>bestScore)
    {
        bestScore=nowScore;
    }
}

void GameManager::resetBestScore()
{
    bestScore=0;
}




