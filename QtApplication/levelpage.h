/*
Author: Léanne Héroux
Date: Febuary 24, 2026
File name: levelpage.h
Description: Code for the page where the levels are selected
Modifications:
*/
#ifndef LEVELPAGE_H
#define LEVELPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class LevelPage : public QWidget
{
    Q_OBJECT

public:
    LevelPage(QWidget *parent = nullptr);

    QPushButton* quitterButton() const { return m_quitterButton; }
    QPushButton* niveau1Button() const { return m_niveau1Button; }
    QPushButton* niveau2Button() const { return m_niveau2Button; }
    QPushButton* niveau3Button() const { return m_niveau3Button; }

private:
    QLabel *m_label;
    QPushButton *m_quitterButton;
    QPushButton *m_niveau1Button;
    QPushButton *m_niveau2Button;
    QPushButton *m_niveau3Button;
};

#endif // LEVELPAGE_H
