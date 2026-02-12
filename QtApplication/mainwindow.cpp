/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: mainwindow.cpp
Description: Code which implements the main window for the Qt application.
Modifications:
    Date: Febuary 2, 2026
        Author: Donavan Sirois
        Description: Coded the paintEvent method which paints polygons to the screen. This
        method is no meant to be permanent and should be deleted eventually.
*/

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupScene();

    m_player.setPosition(0.0f,0.0f);
    m_player.setAngle(0.0f);

    renderWorld();
}

MainWindow::~MainWindow()
{
    delete m_renderer;
    delete m_view;
    delete m_scene;
    delete ui;
}


void MainWindow::setupScene()
{
    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene,this);

    int width = 800;
    int height = 600;
    m_scene->setSceneRect(0,0,width,height);

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setFrameStyle(0);

    setCentralWidget(m_view);

    m_renderer = new WallRenderer(m_scene, width, height);
}
void MainWindow::renderWorld()
{
    m_scene->clear();

    std::vector<Wall> walls;

    // Mur du milieu
    Wall wall1;
    wall1.start = {-5.0f, 10.0f};
    wall1.end = {5.0f, 10.0f};
    wall1.floorHeight = 0.0f;
    wall1.ceilingHeight = 5.0f;
    walls.push_back(wall1);

    // Mur de gauche
    Wall wall2;
    wall2.start = {-5.0f, 10.0f};
    wall2.end = {-5.0f, 5.0f};
    wall2.floorHeight = 0.0f;
    wall2.ceilingHeight = 5.0f;
    walls.push_back(wall2);

    // Mur de droite
    Wall wall3;
    wall3.start = {5.0f, 5.0f};
    wall3.end = {5.0f, 10.0f};
    wall3.floorHeight = 0.0f;
    wall3.ceilingHeight = 5.0f;
    walls.push_back(wall3);

    for (const Wall& wall : walls) {
        m_renderer->renderWall(wall, m_player);
    }
}
