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
    addDoomLabel();
    addPlayButton();
    addLevelButton();
    addPageLayout();
    //ajout a la liste
    menuButtons.append(menu_levelButton);
    menuButtons.append(menu_playButton);
    connectButtons();
    updateHighlight();
    m_background.load(":/ressources/background1.png");
}

void MenuPage::addDoomLabel()
{
    label_doom = new QLabel();
    QPixmap doomLogo(":/ressources/DOOMLOGO.png");
    QPixmap scaled = doomLogo.scaled(700, 350, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    label_doom->setPixmap(scaled);
    label_doom->setAlignment(Qt::AlignCenter);
    label_doom->setStyleSheet("background-color: transparent;");
}

void MenuPage::addPlayButton()
{
    menu_playButton = new QPushButton();
    menu_playButton->setObjectName("bouton");
    menu_playButton->setMinimumSize(450, 120);
    menu_playButton->setAttribute(Qt::WA_Hover);

    menu_playButton->setStyleSheet(
        "QPushButton {"
        "   border-image: url(:/ressources/bouton1.png) 0 0 0 0 stretch stretch;"
        "}"
        "QPushButton:hover {"
        "   border-image: url(:/ressources/bouton2.png) 0 0 0 0 stretch stretch;"
        "}"
        );
}

void MenuPage::addLevelButton()
{
    menu_levelButton = new QPushButton();
    menu_levelButton->setObjectName("niveau");
    menu_levelButton->setMinimumSize(450, 120);
    menu_levelButton->setAttribute(Qt::WA_Hover);

    menu_levelButton->setStyleSheet(        "QPushButton {"
        "   border-image: url(:/ressources/niveau1.png) 0 0 0 0 stretch stretch;"
        "}"
        "QPushButton:hover {"
        "   border-image: url(:/ressources/niveau2.png) 0 0 0 0 stretch stretch;"
        "}"
        );
}

void MenuPage::addPageLayout()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addSpacing(10);
    layout->addWidget(label_doom, 0, Qt::AlignCenter);
    layout->addSpacing(30);
    layout->addWidget(menu_playButton, 0, Qt::AlignCenter);
    layout->addSpacing(30);
    layout->addWidget(menu_levelButton, 0, Qt::AlignCenter);
    layout->addStretch();
}

void MenuPage::connectButtons()
{
    connect(menu_levelButton, &QPushButton::clicked, this, &MenuPage::menu_levelClicked);
    connect(menu_playButton, &QPushButton::clicked, this, &MenuPage::menu_playClicked);
}

void MenuPage::updateHighlight()
{
    for (int i = 0; i < menuButtons.size(); i++) {
        if (menuButtons[i] == menu_playButton)
        {
            menu_playButton->setStyleSheet("QPushButton {border-image: url(:/ressources/bouton1.png) 0 0 0 0 stretch stretch;}");
        }
        if (menuButtons[i] == menu_levelButton)
        {
            menu_levelButton->setStyleSheet("QPushButton {border-image: url(:/ressources/niveau1.png) 0 0 0 0 stretch stretch;}");
        }
    }
    if (currentIndex < menuButtons.size())
    {
        if (menuButtons[currentIndex]==menu_playButton)
        {
            menu_playButton->setStyleSheet("QPushButton {border-image: url(:/ressources/bouton2.png) 0 0 0 0 stretch stretch;}");
        }
        if (menuButtons[currentIndex]==menu_levelButton)
        {
            menu_levelButton->setStyleSheet("QPushButton {border-image: url(:/ressources/niveau2.png) 0 0 0 0 stretch stretch;}");
        }
    }
}

void MenuPage::activateSelectedButton()
{
    //qDebug() << "click selection dans menu";
    if (currentIndex == 0)
    {
        menu_levelClicked();
    }
    else if (currentIndex == 1)
    {
        menu_playClicked();
    }
}

void MenuPage::menu_levelClicked()
{
    //qDebug() << "menu_levelClicked";
    emit menu_levelClickedSig();
}

void MenuPage::menu_playClicked()
{
    //qDebug() << "menu_playClicked";
    emit menu_playClickedSig();
}

void MenuPage::changeButtons()
{
    //qDebug() << "changeButtons";
    currentIndex++;
    if (currentIndex > 1)
    {
        currentIndex = 0;
    }
    //qDebug() << "current index:" << currentIndex;
}

void MenuPage::setupNextSelect()
{
    //qDebug() << "setup Next";
    currentIndex=0;
    updateHighlight();
}

void MenuPage::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), m_background);
    QWidget::paintEvent(event);
}
