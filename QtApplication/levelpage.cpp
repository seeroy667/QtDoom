/*
Author: Léanne Héroux
Date: Febuary 24, 2026
File name: levelpage.cpp
Description: Code for the page where the levels are selected
Modifications:
*/
#include "levelpage.h"

LevelPage::LevelPage(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);
    addTitleLevel();
    addQuitButton();
    addLevelButtons();
    addQuitButtonLayout();
    addLevelButtonsLayout();
    addPageLayout();
    //ajout a la liste
    menuButtons.append(m_quitterButton);
    menuButtons.append(m_niveau1Button);
    menuButtons.append(m_niveau2Button);
    menuButtons.append(m_niveau3Button);
    connectButtons();
    updateHighlight();
    m_background.load(":/ressources/background2.png");
}

void LevelPage::addTitleLevel()
{
    m_label = new QLabel("Choisir un niveau:");
    fontLabel.setPointSize(20);
    m_label->setFont(fontLabel);
    m_label->setAlignment(Qt::AlignCenter);
}

void LevelPage::addQuitButton()
{
    m_quitterButton = new QPushButton("Quitter");
    m_quitterButton->setFixedSize(80, 30);
}

void LevelPage::addLevelButtons()
{
    m_niveau1Button = new QPushButton();
    m_niveau2Button = new QPushButton();
    m_niveau3Button = new QPushButton();
    //style des boutons

    m_niveau1Button->setMinimumSize(450, 450);
    m_niveau1Button->setAttribute(Qt::WA_Hover);

     m_niveau1Button->setStyleSheet(        "QPushButton {"
                                    "   border-image: url(:/ressources/portelvl1.1.png) 0 0 0 0 stretch stretch;"
                                    "}"
                                    "QPushButton:hover {"
                                    "   border-image: url(:/ressources/portelvl1.2.png) 0 0 0 0 stretch stretch;"
                                    "}"
                                    );
    m_niveau2Button->setMinimumSize(450, 450);
    m_niveau2Button->setAttribute(Qt::WA_Hover);

    m_niveau2Button->setStyleSheet(        "QPushButton {"
                                   "   border-image: url(:/ressources/portelvl2.1.png) 0 0 0 0 stretch stretch;"
                                   "}"
                                   "QPushButton:hover {"
                                   "   border-image: url(:/ressources/portelvl2.2.png) 0 0 0 0 stretch stretch;"
                                   "}"
                                   );
    m_niveau3Button->setMinimumSize(450, 450);
    m_niveau3Button->setAttribute(Qt::WA_Hover);

    m_niveau3Button->setStyleSheet(        "QPushButton {"
                                   "   border-image: url(:/ressources/portelvl3.1.png) 0 0 0 0 stretch stretch;"
                                   "}"
                                   "QPushButton:hover {"
                                   "   border-image: url(:/ressources/portelvl3.2.png) 0 0 0 0 stretch stretch;"
                                   "}"
                                   );
}

void LevelPage::addQuitButtonLayout()
{
    topLayout = new QHBoxLayout;
    topLayout->addWidget(m_quitterButton, 0, Qt::AlignLeft);
    topLayout->addStretch();
}

void LevelPage::addLevelButtonsLayout()
{
    levelsLayout = new QHBoxLayout;
    levelsLayout->addStretch();
    levelsLayout->addWidget(m_niveau1Button);
    levelsLayout->addSpacing(20);
    levelsLayout->addWidget(m_niveau2Button);
    levelsLayout->addSpacing(20);
    levelsLayout->addWidget(m_niveau3Button);
    levelsLayout->addStretch();
}

void LevelPage::addPageLayout()
{
    mainLayout->addLayout(topLayout);
    mainLayout->addSpacing(100);
    mainLayout->addWidget(m_label);
    mainLayout->addSpacing(50);
    mainLayout->addLayout(levelsLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

void LevelPage::connectButtons()
{
    connect(m_quitterButton, &QPushButton::clicked, this, &LevelPage::level_quitClicked);
    connect(m_niveau1Button, &QPushButton::clicked, this, &LevelPage::level_1Clicked);
    connect(m_niveau2Button, &QPushButton::clicked, this, &LevelPage::level_2Clicked);
    connect(m_niveau3Button, &QPushButton::clicked, this, &LevelPage::level_3Clicked);
}

void LevelPage::updateHighlight()
{
    //qDebug() << "update light";
    // Réinitialiser les styles de tous les boutons
    for (int i=0; i<menuButtons.size(); i++) {
        if(menuButtons[i] == m_niveau1Button || menuButtons[i] == m_niveau2Button || menuButtons[i] == m_niveau3Button) continue;
        menuButtons[i]->setStyleSheet("background-color: black; color: white;");
    }

    // Appliquer un style au bouton sélectionné
    if (currentIndex < menuButtons.size() && menuButtons[currentIndex] != m_niveau1Button && menuButtons[currentIndex] != m_niveau2Button && menuButtons[currentIndex] != m_niveau3Button)
    {
        menuButtons[currentIndex]->setStyleSheet("background-color: darkgray; color: black;");
    }

}

void LevelPage::activateSelectedButton()
{
    //qDebug() << "click selection dans menu";
    if (currentIndex == 0)
    {
        level_quitClicked();
    }
    else if (currentIndex == 1)
    {
        level_1Clicked();
    }
    else if (currentIndex == 2)
    {
        level_2Clicked();
    }
    else if (currentIndex == 3)
    {
        level_3Clicked();
    }
}

void LevelPage::level_quitClicked()
{
    //qDebug() << "menu_levelClicked";
    emit level_quitClickedSig();
}

void LevelPage::level_1Clicked()
{
    //qDebug() << "level_1Clicked";
    emit level_1ClickedSig();
}

void LevelPage::level_2Clicked()
{
    //qDebug() << "level_2Clicked";
    emit level_2ClickedSig();
}

void LevelPage::level_3Clicked()
{
    //qDebug() << "level_3Clicked";
    emit level_3ClickedSig();
}

void LevelPage::changeButtons()
{
    //qDebug() << "changeButtons";
    currentIndex++;
    if (currentIndex > 3)
    {
        currentIndex = 0;
    }
    //qDebug() << "current index:" << currentIndex;
}

void LevelPage::setupNextSelect()
{
    //qDebug() << "setup Next";
    currentIndex=0;
}

void LevelPage::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), m_background);
    QWidget::paintEvent(event);
}
