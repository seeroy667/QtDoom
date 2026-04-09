#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QString>
#include "menupage.h"
#include "gamepage.h"
#include "levelpage.h"
#include "leaderboard.h"
#include "compte.h"

class UIManager : public QWidget
{
    Q_OBJECT

public:
    UIManager(QGraphicsView *view, QWidget *parent = nullptr);
    GamePage* getGamePage();
    MenuPage* getMenuPage();
    LevelPage* getLevelPage();
    void changeButtons();
    void setupNextSelect();
    void updateHighlight();
    void activateSelectedButton();

private:
    QStackedWidget *stackedWidget;
    MenuPage *menuPage;
    GamePage *gamePage;
    LevelPage *levelPage;
    compte *comptePage;
    leaderBoard *leaderBoardPage;
    bool potTurns=false;
    bool shootPress=false;
    QString pathToLevel = "/../../WadLvl1.txt"; //par défaut load lvl1
    int score = 0;

signals:
    void startGame();
    void keyReleaseSig(QKeyEvent *event);
    void keyPressSig(QKeyEvent *event);
    void pauseGame();
    void loadMap(QString path = "");
    void getScore();

public slots:
    void potIsTurning();
    void potStops();
    void shootPressed();
    void shootReleased();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void updateBalles(int value);
    void updateVie(int value);
    void updateScore(int value);
    void saveLevelPath(QString path = "");
    void goToMenuPage();
    void setScore(int newScore);
};

#endif // UIMANAGER_H
