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
    qDebug() << linedefs.size();
    bsp->build(linedefs, verteces);

    qDebug() << "\n=== COLLECTING ALL WALLS FROM BSP TREE ===";
    std::vector<Linedef> allBSPWalls;
    collectAllWalls(bsp->getRoot(), allBSPWalls);

    qDebug() << "Total walls in BSP tree:" << allBSPWalls.size();
    qDebug() << "\n=== ALL WALLS IN BSP TREE ===";
    for (int i = 0; i < allBSPWalls.size(); i++)
    {
        const Linedef& wall = allBSPWalls[i];

        if (wall.start >= verteces.size() || wall.end >= verteces.size())
        {
            qDebug() << "[" << i << "] INVALID!" << wall.start << "->" << wall.end
                     << "(vertices size:" << verteces.size() << ")";
            continue;
        }

        const Vertex& v1 = verteces[wall.start];
        const Vertex& v2 = verteces[wall.end];

        float dx = v2.x - v1.x;
        float dy = v2.y - v1.y;
        float length = std::sqrt(dx*dx + dy*dy);

        if (length < 0.01f)
        {
            qDebug() << "[" << i << "] DEGENERATE!" << wall.start << "->" << wall.end
                     << "| both at (" << v1.x << "," << v1.y << ")";
        }
        else
        {
            qDebug() << "[" << i << "] Wall" << wall.start << "->" << wall.end
                     << "| (" << v1.x << "," << v1.y << ") to (" << v2.x << "," << v2.y << ")"
                     << "| length:" << length;
        }
    }

    qDebug() << "======================================\n";

    cManager = new CollisionManager();

    qDebug() << "=== VERTICES ===";
    for (int i = 0; i < std::min(30, (int)verteces.size()); i++)
    {
        qDebug() << "v" << i << ":" << verteces[i].x << verteces[i].y;
    }

    qDebug() << "\n=== LINEDEFS ===";
    for (int i = 0; i < linedefs.size(); i++)
    {
        qDebug() << "linedef" << i << ":" << linedefs[i].start << "->" << linedefs[i].end;
    }
}

BSP* GameManager::getBSP()
{
    return bsp;
}

void GameManager::update(float deltaTime, std::vector<Linedef> renderedWalls)
{
    e->moveEnemy(*p, deltaTime);

    qDebug() << "\n========== FRAME DEBUG ==========";
    qDebug() << "Player position:" << p->getPosition().x << p->getPosition().y;

    // Debug: Print ALL linedefs in the map
    qDebug() << "\n=== ALL LINEDEFS IN MAP ===";
    for (int i = 0; i < linedefs.size(); i++)
    {
        const Linedef& ld = linedefs[i];
        const Vertex& v1 = verteces[ld.start];
        const Vertex& v2 = verteces[ld.end];
        qDebug() << "  linedef" << i << ":" << ld.start << "->" << ld.end
                 << "| v1:(" << v1.x << "," << v1.y << ")"
                 << "| v2:(" << v2.x << "," << v2.y << ")";
    }

    std::vector<Linedef> broadedWalls;

    // Collision detection
    bsp->actorToWallBroading(p->getPosition(), broadedWalls, verteces);

    // Debug: Print broaded walls
    qDebug() << "\n=== BROADED WALLS (from BSP) ===";
    qDebug() << "Count:" << broadedWalls.size();
    for (int i = 0; i < broadedWalls.size(); i++)
    {
        const Linedef& wall = broadedWalls[i];

        // Check if indices are valid
        if (wall.start >= verteces.size() || wall.end >= verteces.size())
        {
            qDebug() << "  [" << i << "] INVALID INDICES!" << wall.start << "->" << wall.end
                     << "(vertices size:" << verteces.size() << ")";
            continue;
        }

        const Vertex& v1 = verteces[wall.start];
        const Vertex& v2 = verteces[wall.end];

        // Calculate distance to wall
        float dxWall = v2.x - v1.x;
        float dyWall = v2.y - v1.y;
        float dxPlayer = p->getPosition().x - v1.x;
        float dyPlayer = p->getPosition().y - v1.y;

        float cross = dxWall * dyPlayer - dyWall * dxPlayer;
        float wallLength = std::sqrt(dxWall * dxWall + dyWall * dyWall);
        float distanceToLine = (wallLength > 0.001f) ? std::abs(cross / wallLength) : 999.0f;

        qDebug() << "  [" << i << "] Wall" << wall.start << "->" << wall.end
                 << "| v1:(" << v1.x << "," << v1.y << ")"
                 << "| v2:(" << v2.x << "," << v2.y << ")"
                 << "| distance:" << distanceToLine;
    }

    cManager->narrowingToCollide(broadedWalls, verteces, p);

    for (Actor* creature : creatures)
    {
        cManager->narrowingToCollide(linedefs, verteces, creature);
    }

    // Enemy damage detection
    if (inRadius(p, e))
    {

        if (e->getHealth() == 0) return;

        if(!m_inContact)
        {
            m_inContact = true;
            m_enemyAttackTimer.restart();
            p->takeDamage(1);
            updateVie();
        }
        else if(m_enemyAttackTimer.elapsed() >= m_attackCooldown)
        {
            m_inContact = false;
            p->takeDamage(1);
            updateVie();
            m_enemyAttackTimer.restart();
        }

        if (p->getHealth() < 1)
        {
            //qDebug("Player Dead");
            emit playerDead();
        }
    }
    else
    {
        e->setMovement(true);
        m_inContact = false;
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

