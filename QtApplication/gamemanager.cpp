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
        {{-5.0f, 10.0f}, {5.0f, 10.0f}, 0.0f, 5.0f},
        {{-5.0f, 10.0f}, {-5.0f, 5.0f}, 0.0f, 5.0f},
        {{5.0f, 5.0f}, {5.0f, 10.0f}, 0.0f, 5.0f}
    };

    bsp = new BSP();
    bsp->build(walls);
}

BSP* GameManager::getBSP()
{
    return bsp;
}
