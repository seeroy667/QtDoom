/*
Author: Léanne Héroux
Date: Febuary 24, 2026
File name: levelpage.cpp
Description: Code for the page where the levels are selected
Modifications:
*/
#include "levelpage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>

LevelPage::LevelPage(QWidget *parent)
    : QWidget(parent)
{
    //titre de la page
    m_label = new QLabel("Choisir un niveau:");
    QFont fontLabel;
    fontLabel.setPointSize(20);
    m_label->setFont(fontLabel);
    m_label->setAlignment(Qt::AlignCenter);

    //bouton quitter
    m_quitterButton = new QPushButton("Quitter");
    m_quitterButton->setFixedSize(80, 30);
    //layout du bouton quitter
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(m_quitterButton, 0, Qt::AlignLeft);
    topLayout->addStretch();

    //boutons pour les niveaux
    m_niveau1Button = new QPushButton("Niveau 1");
    m_niveau2Button = new QPushButton("Niveau 2");
    m_niveau3Button = new QPushButton("Niveau 3");
    //layout des boutons level
    QHBoxLayout *levelsLayout = new QHBoxLayout;
    levelsLayout->addStretch();
    levelsLayout->addWidget(m_niveau1Button);
    levelsLayout->addSpacing(20);
    levelsLayout->addWidget(m_niveau2Button);
    levelsLayout->addSpacing(20);
    levelsLayout->addWidget(m_niveau3Button);
    levelsLayout->addStretch();
    //style des boutons
    QFont fontButton;
    fontButton.setPointSize(20);
    m_niveau1Button->setFont(fontButton);
    m_niveau2Button->setFont(fontButton);
    m_niveau3Button->setFont(fontButton);
    m_niveau1Button->setFixedSize(150, 120);
    m_niveau2Button->setFixedSize(150, 120);
    m_niveau3Button->setFixedSize(150, 120);

    //layout de la page
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addSpacing(100);
    mainLayout->addWidget(m_label);
    mainLayout->addSpacing(50);
    mainLayout->addLayout(levelsLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

