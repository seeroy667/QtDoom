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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Engine.h"
#include <QMainWindow>
#include <QStackedWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Engine *engine  = nullptr;
};

#endif // MAINWINDOW_H



