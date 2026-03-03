/*
Author: Léanne Héroux
Date: Febuary 24, 2026
File name: gamepage.cpp
Description: Code for the page where the game is played
Modifications:
*/
#include "gamepage.h"

GamePage::GamePage(QGraphicsView *view, QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    //bouton quitter
    QHBoxLayout *quitLayout = new QHBoxLayout;
    m_quitterButton = new QPushButton("Quitter");
    quitLayout->addWidget(m_quitterButton, 0, Qt::AlignLeft);
    mainLayout->addLayout(quitLayout);

    //layout du centre
    QWidget *centralWidget = new QWidget;
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);

    //ajouter le jeu à l'application
    centralLayout->addWidget(view);

    //pour la page gameover: manque juste à le centrer
    //titre gameover
    QLabel *labelGameOver = new QLabel("Game Over");
    labelGameOver->setAlignment(Qt::AlignCenter);
    QFont font;
    font.setPointSize(20);
    labelGameOver->setFont(font);

    //bouton retry
    m_retryButton = new QPushButton("Retry");
    m_retryButton->setStyleSheet("background-color:black; color:white;");
    m_retryButton->setFixedSize(80,25);

    //widget du gameover
    m_gameOverWidget = new QWidget(centralWidget);
    m_gameOverWidget->setFixedSize(300, 200);
    m_gameOverWidget->setStyleSheet("background-color:red; border-radius:5px;");
    QVBoxLayout *gameOverLayout = new QVBoxLayout(m_gameOverWidget);

    //layout pour le popup
    gameOverLayout->addStretch();
    gameOverLayout->addWidget(labelGameOver);
    gameOverLayout->addSpacing(10);
    gameOverLayout->addWidget(m_retryButton, 0, Qt::AlignCenter);
    gameOverLayout->addStretch();

    //cache le game over popup
    m_gameOverWidget->hide();

    mainLayout->addWidget(centralWidget);

    //barre d'état au bas de l'écran:
    QWidget *barWidget = new QWidget;
    barWidget->setStyleSheet("background-color: gray;");
    barWidget->setFixedHeight(100);
    QHBoxLayout *barLayout = new QHBoxLayout(barWidget);

    //barre de vie du joueur
    QLabel *labelVie = new QLabel("vie:");
    m_barVie = new QProgressBar;
    m_barVie->setFixedHeight(30);
    m_barVie->setFixedWidth(200);
    m_barVie->setTextVisible(false);
    m_barVie->setStyleSheet("QProgressBar{background-color:lightgrey; border: 2px solid black; border-radius: 5px;} QProgressBar::chunk{background:green;}");
    m_barVie->setValue(80); //a modifier

    //nb de balles tirées
    QLabel *labelAmo = new QLabel("balles:");
    m_writeAmo = new QLineEdit;
    m_writeAmo->setFixedWidth(40);
    m_writeAmo->setReadOnly(true);
    m_writeAmo->setText(QString::number(10)); //a modifier

    //bouton tirer (à retirer)
    m_shootButton = new QPushButton("SHOOT");
    m_shootButton->setFixedSize(100, 50);
    m_shootButton->setStyleSheet("background-color:red;");

    //layout de la barre d'état
    barLayout->addStretch();
    barLayout->addWidget(labelVie);
    barLayout->addWidget(m_barVie);
    barLayout->addSpacing(100);
    barLayout->addWidget(labelAmo);
    barLayout->addWidget(m_writeAmo);
    barLayout->addStretch();
    mainLayout->addWidget(barWidget);
}

void GamePage::keyPressEvent(QKeyEvent *event)
{
    emit keyPressSig(event);
}

void GamePage::keyReleaseEvent(QKeyEvent *event)
{
    emit keyReleaseSig(event);
}

void GamePage::showEvent(QShowEvent *event)
{
}

