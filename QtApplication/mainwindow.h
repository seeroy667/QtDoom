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
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QtGui>
#include <QtCore>
#include "wallrenderer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_scene;
    QGraphicsView *m_view;
    WallRenderer *m_renderer;
    Player m_player;

    void setupScene();
    void renderWorld();
};
#endif // MAINWINDOW_H
