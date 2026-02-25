#ifndef UIMANAGER_H
#define UIMANAGER_H

#include"menupage.h"
#include"levelpage.h"

class UIManager
{
public:
    UIManager();
private:
    MenuPage* mainMenu;
    //GamePage* gameUI;
    LevelPage* levelSelector;
};

#endif // UIMANAGER_H
