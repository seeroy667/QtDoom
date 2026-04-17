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
    //addTitleLevel();
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
    m_background.load(":/ressources/background2.jpg");
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
    m_quitterButton = new QPushButton();
    m_quitterButton->setFixedSize(160, 60);
    m_quitterButton->setAttribute(Qt::WA_Hover);
    m_quitterButton->setStyleSheet(        "QPushButton {"
                                   "   border-image: url(:/ressources/retour1.png) 0 0 0 0 stretch stretch;"
                                   "}"
                                   "QPushButton:hover {"
                                   "   border-image: url(:/ressources/retour2.png) 0 0 0 0 stretch stretch;"
                                   "}"
                                   );
}

void LevelPage::addLevelButtons()
{
    m_niveau1Button = new QPushButton();
    m_niveau2Button = new QPushButton();
    m_niveau3Button = new QPushButton();
    //style des boutons

    m_niveau1Button->setMinimumSize(350, 350);
    m_niveau1Button->setAttribute(Qt::WA_Hover);

     m_niveau1Button->setStyleSheet(        "QPushButton {"
                                    "   border-image: url(:/ressources/portelvl1.1.png) 0 0 0 0 stretch stretch;"
                                    "}"
                                    "QPushButton:hover {"
                                    "   border-image: url(:/ressources/portelvl1.2.png) 0 0 0 0 stretch stretch;"
                                    "}"
                                    );
    m_niveau2Button->setMinimumSize(350, 350);
    m_niveau2Button->setAttribute(Qt::WA_Hover);

    m_niveau2Button->setStyleSheet(        "QPushButton {"
                                   "   border-image: url(:/ressources/portelvl2.1.png) 0 0 0 0 stretch stretch;"
                                   "}"
                                   "QPushButton:hover {"
                                   "   border-image: url(:/ressources/portelvl2.2.png) 0 0 0 0 stretch stretch;"
                                   "}"
                                   );
    m_niveau3Button->setMinimumSize(350, 350);
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
    //mainLayout->addWidget(m_label);
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
    // Remet tous les boutons à leur état normal
    m_quitterButton->setStyleSheet(
        "QPushButton {"
        "   border-image: url(:/ressources/retour1.png) 0 0 0 0 stretch stretch;"
        "}"
        "QPushButton:hover {"
        "   border-image: url(:/ressources/retour2.png) 0 0 0 0 stretch stretch;"
        "}"
        );
    m_niveau1Button->setStyleSheet(
        "QPushButton {"
        "   border-image: url(:/ressources/portelvl1.1.png) 0 0 0 0 stretch stretch;"
        "}"
        "QPushButton:hover {"
        "   border-image: url(:/ressources/portelvl1.2.png) 0 0 0 0 stretch stretch;"
        "}"
        );
    m_niveau2Button->setStyleSheet(
        "QPushButton {"
        "   border-image: url(:/ressources/portelvl2.1.png) 0 0 0 0 stretch stretch;"
        "}"
        "QPushButton:hover {"
        "   border-image: url(:/ressources/portelvl2.2.png) 0 0 0 0 stretch stretch;"
        "}"
        );
    m_niveau3Button->setStyleSheet(
        "QPushButton {"
        "   border-image: url(:/ressources/portelvl3.1.png) 0 0 0 0 stretch stretch;"
        "}"
        "QPushButton:hover {"
        "   border-image: url(:/ressources/portelvl3.2.png) 0 0 0 0 stretch stretch;"
        "}"
        );

    // Applique l'image hover sur le bouton sélectionné par la manette
    if (currentIndex == 0)
    {
        m_quitterButton->setStyleSheet(
            "QPushButton {"
            "   border-image: url(:/ressources/retour2.png) 0 0 0 0 stretch stretch;"
            "}"
            );
    }
    else if (currentIndex == 1)
    {
        m_niveau1Button->setStyleSheet(
            "QPushButton {"
            "   border-image: url(:/ressources/portelvl1.2.png) 0 0 0 0 stretch stretch;"
            "}"
            );
    }
    else if (currentIndex == 2)
    {
        m_niveau2Button->setStyleSheet(
            "QPushButton {"
            "   border-image: url(:/ressources/portelvl2.2.png) 0 0 0 0 stretch stretch;"
            "}"
            );
    }
    else if (currentIndex == 3)
    {
        m_niveau3Button->setStyleSheet(
            "QPushButton {"
            "   border-image: url(:/ressources/portelvl3.2.png) 0 0 0 0 stretch stretch;"
            "}"
            );
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
    emit chosenLevelPath("/../../WadLvl1.txt");
}

void LevelPage::level_2Clicked()
{
    //qDebug() << "level_2Clicked";
    emit chosenLevelPath("/../../WadLvl2.txt");
}

void LevelPage::level_3Clicked()
{
    //qDebug() << "level_3Clicked";
    emit chosenLevelPath("/../../WadLvl3.txt");
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
    updateHighlight();
}

void LevelPage::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), m_background);
    QWidget::paintEvent(event);
}
