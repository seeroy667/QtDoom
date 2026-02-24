/*
Author: Léanne Héroux
Date: Febuary 24, 2026
File name: gamepage.h
Description: Code for the page where the game is played
Modifications:
*/
#ifndef GAMEPAGE_H
#define GAMEPAGE_H
#include "engine.h"
#include <QWidget>
#include <QPushButton>
#include <QPushButton>
#include <QLineEdit>
#include <QProgressBar>
#include <QStackedWidget>

class GamePage : public QWidget
{
    Q_OBJECT

public:
    explicit GamePage(QWidget *parent = nullptr);

    // Widgets importants accessibles
    QPushButton* quitterButton() const { return m_quitterButton; }
    QPushButton* retryButton() const { return m_retryButton; }
    QPushButton* shootButton() const { return m_shootButton; }
    QProgressBar* vieBar() const { return m_barVie; }
    QLineEdit* amoEdit() const { return m_writeAmo; }

    // Game Over popup
    QWidget* gameOverPopup() const { return m_gameOverWidget; }

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Engine *engine;
    // Boutons
    QPushButton *m_quitterButton;
    QPushButton *m_retryButton;
    QPushButton *m_shootButton;

    // HUD
    QProgressBar *m_barVie;
    QLineEdit *m_writeAmo;

    // Game Over
    QWidget *m_gameOverWidget;
    QStackedWidget *m_stackedWidget;
};
#endif // GAMEPAGE_H
