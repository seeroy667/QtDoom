#include "uimanager.h"

UIManager::UIManager(QGraphicsView *view, QWidget *parent): QWidget(parent) {

    QVBoxLayout *layout = new QVBoxLayout(this);
    stackedWidget = new QStackedWidget;
    layout->addWidget(stackedWidget);
    setCursor(Qt::CrossCursor);
    // Créer les pages
    menuPage = new MenuPage;
    gamePage = new GamePage(view);
    levelPage = new LevelPage;
    comptePage = new compte;
    QString mapPath = QCoreApplication::applicationDirPath() + "/../../scores.txt";
    leaderBoardPage = new leaderBoard(mapPath);

    // Ajouter les pages au stacked widget
    stackedWidget->addWidget(menuPage);
    stackedWidget->addWidget(gamePage);
    stackedWidget->addWidget(levelPage);
    stackedWidget->addWidget(comptePage);
    stackedWidget->addWidget(leaderBoardPage);

    // Page de départ
    stackedWidget->setCurrentWidget(comptePage);

    // Connexions des boutons
    connect(menuPage, &MenuPage::menu_playClickedSig, this, [this]() {
        //qDebug() << "connect ok";
        stackedWidget->setCurrentWidget(gamePage);
        emit loadMap(pathToLevel);
        emit startGame();
        //qDebug() << "le jeu commence";
    });


    connect(menuPage, &MenuPage::menu_levelClickedSig, this, [this]() {
        stackedWidget->setCurrentWidget(levelPage);
    });

    connect(gamePage, &GamePage::over_quitClickedSig, this, [this]() {
        stackedWidget->setCurrentWidget(menuPage);
    });

    connect(gamePage, &GamePage::menu_quitClickedSig, this, [this]() {
        stackedWidget->setCurrentWidget(menuPage);
    });

    connect(levelPage, &LevelPage::level_quitClickedSig, this, [this]() {
        stackedWidget->setCurrentWidget(menuPage);
    });

    connect(menuPage, &MenuPage::loginClickedSig, this, [this]() {
        stackedWidget->setCurrentWidget(comptePage);
        QString username = comptePage->getCurrentUsername();
        emit getScore();
        qDebug() << score;
        leaderBoardPage->saveScore(username,score);
        emit newPlayer();
    });

    connect(comptePage, &compte::annulerChangeUser, this, [this]() {
        stackedWidget->setCurrentWidget(menuPage);
        QString username=comptePage->getCurrentUsername();
        qDebug()<< username;
    });

    connect(levelPage, SIGNAL(chosenLevelPath(QString)), this, SLOT(saveLevelPath(QString)));
    connect(comptePage, SIGNAL(loginSig()), this, SLOT(goToMenuPage()));
    connect(leaderBoardPage, SIGNAL(goBackToMenu()), this, SLOT(goToMenuPage()));
    connect(menuPage, SIGNAL(scoreClickedSig()), this, SLOT(goToLeaderBoard()));
}

void UIManager::setScore(int newScore)
{
    score=newScore;
}

void UIManager::goToMenuPage()
{
    stackedWidget->setCurrentWidget(menuPage);
}

void UIManager::goToLeaderBoard()
{
    stackedWidget->setCurrentWidget(leaderBoardPage);
    QString username = comptePage->getCurrentUsername();
    emit getScore();
    leaderBoardPage->saveScore(username,score);
    leaderBoardPage->load10BestPlayers();
}

void UIManager::saveScoreBeforeNextLevel()
{
    QString username = comptePage->getCurrentUsername();
    emit getScore();
    leaderBoardPage->saveScore(username,score);
}

GamePage* UIManager::getGamePage()
{
    return gamePage;
}

MenuPage* UIManager::getMenuPage()
{
    return menuPage;
}

LevelPage* UIManager::getLevelPage()
{
    return levelPage;
}

void UIManager::changeButtons()
{
    if(stackedWidget->currentWidget()==menuPage)
    {
        getMenuPage()->changeButtons();
    }
    if(stackedWidget->currentWidget()==gamePage)
    {
        getGamePage()->changeButtons();
    }
    if(stackedWidget->currentWidget()==levelPage)
    {
        getLevelPage()->changeButtons();
    }
}

void UIManager::setupNextSelect()
{
    if(stackedWidget->currentWidget()==menuPage)
    {
        getMenuPage()->setupNextSelect();
    }
    if(stackedWidget->currentWidget()==gamePage)
    {
        getGamePage()->setupNextSelect();
    }
    if(stackedWidget->currentWidget()==levelPage)
    {
        getLevelPage()->setupNextSelect();
    }
}
void UIManager::updateHighlight()
{
    if(stackedWidget->currentWidget()==menuPage)
    {
        getMenuPage()->updateHighlight();
    }
    if(stackedWidget->currentWidget()==gamePage)
    {
        getGamePage()->updateHighlight();
    }
    if(stackedWidget->currentWidget()==levelPage)
    {
        getLevelPage()->updateHighlight();
    }
    if(stackedWidget->currentWidget()==leaderBoardPage)
    {
        leaderBoardPage->selectQuitterButton();
    }
}
void UIManager::activateSelectedButton()
{
    if(stackedWidget->currentWidget()==menuPage)
    {
        //qDebug() << "selection ui menu";
        getMenuPage()->activateSelectedButton();
        return;
    }
    if(stackedWidget->currentWidget()==gamePage)
    {
        //qDebug() << "selection ui game";
        getGamePage()->activateSelectedButton();
        return;
    }
    if(stackedWidget->currentWidget()==levelPage)
    {
        //qDebug() << "selection ui level";
        getLevelPage()->activateSelectedButton();
        return;
    }
    if(stackedWidget->currentWidget()==leaderBoardPage)
    {
        leaderBoardPage->activateSelectedButton();
        return;
    }
    if(stackedWidget->currentWidget()==comptePage)
    {
        comptePage->activateSelectedButton();
        return;
    }
}

void UIManager::potIsTurning()
{
    potTurns=true;
    if (stackedWidget->currentWidget()==getGamePage() && getGamePage()->gameIsOn()==true)
    {
        //qDebug() << "show popup";
        getGamePage()->showMenuPopup();
        emit pauseGame();
    }
    else
    {
        changeButtons();
    }
}

void UIManager::potStops()
{
    potTurns=false;
    updateHighlight();
}

void UIManager::shootPressed()
{
    shootPress=true;
    activateSelectedButton();
    setupNextSelect();
}

void UIManager::shootReleased()
{
    shootPress=false;
}

void UIManager::keyPressEvent(QKeyEvent *event)
{
    //qDebug() << "touche de ui pesee";
    emit keyPressSig(event);
}

void UIManager::keyReleaseEvent(QKeyEvent *event)
{
    //qDebug() << "touche de ui relache";
    emit keyReleaseSig(event);
}

void UIManager::updateBalles(int value)
{
    getGamePage()->updateBalles(value);
}

void UIManager::updateVie(int value)
{
    getGamePage()->updateVie(value);
}

void UIManager::updateScore(int value)
{
    getGamePage()->updateScore(value);
}

void UIManager::updatePowerUp(float value)
{
    getGamePage()->updatePowerUp(value);
}

void UIManager::saveLevelPath(QString path)
{
    stackedWidget->setCurrentWidget(menuPage);
    pathToLevel=path;
}
