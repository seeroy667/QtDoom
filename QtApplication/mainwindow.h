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

#include <QMainWindow>
#include <QStackedWidget>
#include "menupage.h"
#include "gamepage.h"
#include "levelpage.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QStackedWidget *stackedWidget;
    MenuPage *menuPage;
    GamePage *gamePage;
    LevelPage *levelPage;
};

#endif // MAINWINDOW_H



