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

    QGraphicsScene *scene = new QGraphicsScene(this);
    QGraphicsView  *view  = new QGraphicsView(scene, this);

    scene->setSceneRect(0, 0, 800, 600);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFrameStyle(0);
    setCentralWidget(view);  // This is what makes it visible

    engine = new Engine(scene, 800, 600, this);
    engine->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    engine->getcManager()->keyPressedEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    engine->getcManager()->keyReleasedEvent(event);
}
