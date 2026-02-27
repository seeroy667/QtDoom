#include "uimanager.h"

UIManager::UIManager(QGraphicsView *view, QWidget *parent): QWidget(parent) {

    QVBoxLayout *layout = new QVBoxLayout(this);
    stackedWidget = new QStackedWidget;
    layout->addWidget(stackedWidget);

    // Créer les pages
    menuPage = new MenuPage;
    gamePage = new GamePage(view);
    levelPage = new LevelPage;

    // Ajouter les pages au stacked widget
    stackedWidget->addWidget(menuPage);
    stackedWidget->addWidget(gamePage);
    stackedWidget->addWidget(levelPage);

    // Page de départ
    stackedWidget->setCurrentWidget(menuPage);

    // Connexions des boutons
    connect(menuPage->playButton(), &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(gamePage);
        emit startGame();
    });

    connect(gamePage->quitterButton(), &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(menuPage);
    });

    connect(levelPage->quitterButton(), &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(menuPage);
    });

    connect(menuPage->levelButton(), &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(levelPage);
    });
}

GamePage* UIManager::getGamePage()
{
    return gamePage;
}
