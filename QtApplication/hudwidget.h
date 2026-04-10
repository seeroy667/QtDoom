#pragma once
#include <QWidget>
#include <QPainter>
#include <QPixmap>

class HudWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HudWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        m_bg.load(":/ressources/hud.png");
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        painter.drawPixmap(0, 0, width(), height(), m_bg);
    }

private:
    QPixmap m_bg;
};
