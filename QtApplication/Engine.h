/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: Engine.h
Description: Header file for the game engine handling all events.
Modifications:
*/

#ifndef ENGINE_H
#define ENGINE_H

#include<QObject>
#include<QTimer>
#include<QElapsedTimer>
#include<QGraphicsScene>
#include<QGraphicsView>
#include<QDebug>
#include<QPoint>

#include"controllermanager.h"
#include"gamemanager.h"
#include"rendermanager.h"
#include"uimanager.h"

#define TARGET_FPS 60

class Engine : public QObject{
    Q_OBJECT
private:
    ControllerManager *cManager;
    GameManager *gManager;
    RenderManager *rManager;
    UIManager *uiManager;

    // To operate the frames
    QTimer timer;
    QElapsedTimer elapsedTimer;
    float deltaTime = 0.0f;

    int m_width;
    int m_height;
    QWidget* m_widget;

public:
    Engine(QGraphicsScene *scene, QWidget* widget, int width, int height, QObject *parent);
	~Engine();
    void start();
    void gameLoop();
    ControllerManager* getcManager() const;
};

#endif
