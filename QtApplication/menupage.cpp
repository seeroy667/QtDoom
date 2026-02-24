/*
Author: Léanne Héroux
Date: Febuary 24, 2026
File name: menupage.cpp
Description: Code for the menu page of the game
Modifications:
*/
#include "menupage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>

MenuPage::MenuPage(QWidget *parent)
    : QWidget(parent)
{
    QLabel *label_doom = new QLabel("DOOM");
    QFont font;
    font.setPointSize(36);
    font.setBold(true);
    label_doom->setFont(font);

    m_playButton = new QPushButton("Play");
    m_playButton->setMinimumSize(150, 40);
    m_playButton->setStyleSheet("border-width: 4px; border-style: solid; border-color: green; color:black;");

    m_levelButton = new QPushButton("Niveaux");
    m_levelButton->setMinimumSize(80, 25);
    m_levelButton->setStyleSheet("border-width: 1px; border-style: solid; border-color: black; color:black;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(label_doom, 0, Qt::AlignCenter);
    layout->addSpacing(30);
    layout->addWidget(m_levelButton, 0, Qt::AlignCenter);
    layout->addSpacing(30);
    layout->addWidget(m_playButton, 0, Qt::AlignCenter);
    layout->addStretch();
}

