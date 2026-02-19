#include "gamemanager.h"

GameManager::GameManager() {
    p = new Actor();
    p->setPosition(0.0f, 0.0f);
    p->setAngle(0.0f);
}

Actor* GameManager::getPlayer()
{
    return p;
}
