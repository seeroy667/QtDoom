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

#include"Controller.h"
#include"gamemanager.h"
#include"painter.h"
#include"Player.h"

#define TARGET_FPS 60

class Engine : public QObject{
    Q_OBJECT
private:
    Controller *cManager;
    GameManager *gManager;
    WallRenderer *rManager;
    Player *p; // A SUPPRIMER EVENTUELLEMENT
    // UI MANAGER

    // To operate the frames
    QTimer timer;
    QElapsedTimer elapsedTimer;
public:
    Engine(QGraphicsScene *scene, int width, int height, QObject *parent);
	~Engine();
    void start();
    void gameLoop();
};

#endif
