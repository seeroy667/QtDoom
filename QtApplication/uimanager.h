#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QMainWindow>
#include <QStackedWidget>
#include "menupage.h"
#include "gamepage.h"
#include "levelpage.h"

class UIManager : public QWidget
{
    Q_OBJECT

public:
    UIManager(QGraphicsView *view, QWidget *parent = nullptr);
    GamePage* getGamePage();

private:
    QStackedWidget *stackedWidget;
    MenuPage *menuPage;
    GamePage *gamePage;
    LevelPage *levelPage;

signals:
    void startGame();
};

#endif // UIMANAGER_H
