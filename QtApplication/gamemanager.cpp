#include "gamemanager.h"

GameManager::GameManager() {
    p = new Actor();
    p->setPosition(0.0f, 0.0f);
    p->setAngle(0.0f);

//temporaire a modifier
    e = new Actor();
    e->setPosition(5.0f,5.0f);
    e->setAngle(0.0f);
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
    // Hardcoded for now, replace with filename when you want to generate the map from the file.
    walls = {
        // Outer square (60x60 centered at origin)
        {{-30.0f, 30.0f}, {30.0f, 30.0f}, 0.0f, 20.0f},    // North wall (top)
        {{30.0f, 30.0f}, {30.0f, -30.0f}, 0.0f, 20.0f},    // East wall (right)
        {{30.0f, -30.0f}, {-30.0f, -30.0f}, 0.0f, 20.0f},  // South wall (bottom)
        {{-30.0f, -30.0f}, {-30.0f, 30.0f}, 0.0f, 20.0f},  // West wall (left)

        // Inner square obstacle (10x10 offset to the side)
        {{10.0f, 15.0f}, {20.0f, 15.0f}, 0.0f, 20.0f},     // Inner north
        {{20.0f, 15.0f}, {20.0f, 5.0f}, 0.0f, 20.0f},      // Inner east
        {{20.0f, 5.0f}, {10.0f, 5.0f}, 0.0f, 20.0f},       // Inner south
        {{10.0f, 5.0f}, {10.0f, 15.0f}, 0.0f, 20.0f}       // Inner west
    };
    bsp = new BSP();
    bsp->build(walls);
}

BSP* GameManager::getBSP()
{
    return bsp;
}

void GameManager::update(float deltaTime, std::vector<Linedef> renderedWalls)
{
    e->moveEnemy(*p, deltaTime);

    // Collision detection
    for (const Linedef& wall : renderedWalls) {
        float ok = 0;
    }

    // Enemy damage detection
    if (inRadius(p, e))
    {
        p->takeDamage(1);
        if (p->getHealth() < 1)
        {
            qDebug("Player Dead");
        }
    }
}

bool GameManager::inRadius(Actor* p, Actor* e)
{
    float radius = 0.3f;
    float dx = p->getPosition().x - e->getPosition().x;
    float dy = p->getPosition().y - e->getPosition().y;

    float distance = (dx * dx) + (dy * dy);
    if (distance < (radius * radius)) return true;
    return false;
}

void GameManager::shoot(QPoint mousePos)
{
    for (Actor* e : creatures)
    {
        if (mousePos.x() < e->getPosition().x+1000 && mousePos.x() > e->getPosition().x - 1000 && mousePos.y() < e->getPosition().y + 1000 && mousePos.y() > e->getPosition().y - 1000)
        {
            e->takeDamage(100);
            qDebug("Enemy killed");
        }
    }
}
