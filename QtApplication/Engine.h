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
    // Managers used to communicate
    ControllerManager *cManager;
    GameManager *gManager;
    RenderManager *rManager;
    UIManager *uiManager;

    // To operate the frames
    QTimer timer;
    QElapsedTimer elapsedTimer;
    float deltaTime = 0.0f;

    // What is that
    int m_width;
    int m_height;
    QWidget* m_widget;
    QString oldMap = "";

    // Used to render on screen
    QGraphicsScene* m_scene;
    QGraphicsView*  m_view;

    // Used in for aiming and cursor to screen border rotation
    static constexpr int CURSOR_MIN = 0;
    static constexpr int CURSOR_MAX = 255;
    float m_smoothX = -1.0f;
    float m_smoothY = -1.0f;
    static constexpr float CURSOR_LERP_SPEED = 8.0f;
    static constexpr int   BORDER_ZONE       = 40;
    static constexpr float BORDER_ROT_SPEED  = 2.0f;

public:
    Engine(QGraphicsScene *scene, int width, int height, QObject *parent, QGraphicsView *view);
    ~Engine();
    void gameLoop();
    ControllerManager* getcManager() const;
    UIManager* getuiManager() const;

public slots:
    void start();
    void pauseGame();
    void resumeGame();
    void restartGame();
    void quitGame();
    void gameOver();
    void loadMapIntoGame(QString path = "");
    void onWeaponChanged();
};

#endif
