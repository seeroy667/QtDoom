/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: mainwindow.cpp
Description: Header file which implements the main window for the Qt application.
Modifications:
    Date: Febuary 2, 2026
        Author: Donavan Sirois
        Description: Added the paintEvent method which paints polygons to the screen. This
        method is no meant to be permanent and should be deleted eventually.
    Date: Febuary 24, 2026
        Author: Léanne Héroux
        Description: Added the gamePage the menuPage and the levelPage to the mainWindow
*/
#include "mainwindow.h"

#include <QStackedWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800, 600); //aggrandi un peu la page par défaut

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *layout = new QVBoxLayout(central);

    stackedWidget = new QStackedWidget;
    layout->addWidget(stackedWidget);

    // Créer les pages
    menuPage = new MenuPage;
    gamePage = new GamePage;
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

MainWindow::~MainWindow()
{}
