/*
Author: Léanne Héroux
Date: Febuary 24, 2026
File name: menupage.h
Description: Code for the menu page of the game
Modifications:
*/
#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <QWidget>
#include <QPushButton>

class MenuPage : public QWidget
{
    Q_OBJECT
public:
    explicit MenuPage(QWidget *parent = nullptr);

    QPushButton* playButton() const { return m_playButton; }
    QPushButton* levelButton() const { return m_levelButton; }

private:
    QPushButton *m_playButton;
    QPushButton *m_levelButton;
};

#endif // MENUPAGE_H
