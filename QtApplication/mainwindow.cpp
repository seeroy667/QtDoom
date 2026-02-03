/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: mainwindow.cpp
Description: Code which implements the main window for the Qt application.
Modifications:
    Date: Febuary 2, 2026
        Author: Donavan Sirois
        Description: Coded the paintEvent method which paints polygons to the screen. This
        method is no meant to be permanent and should be deleted eventually.
*/

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *e) // Should eventually be replaced by a universal rendering class.
{
    QPainter painter(this);

    QPolygon poly1;
    poly1 << QPoint(10, 10);
    poly1 << QPoint(10, 500);
    poly1 << QPoint(300, 400);
    poly1 << QPoint(300, 200);

    QPolygon poly2;
    poly2 << QPoint(300, 400);
    poly2 << QPoint(300, 200);
    poly2 << QPoint(500, 200);
    poly2 << QPoint(500, 400);

    QPolygon poly3;
    poly3 << QPoint(500, 200);
    poly3 << QPoint(500, 400);
    poly3 << QPoint(800, 500);
    poly3 << QPoint(800, 10);

    QPolygon poly4;
    poly4 << QPoint(300, 200);
    poly4 << QPoint(500, 200);
    poly4 << QPoint(800, 10);
    poly4 << QPoint(10, 10);

    QPolygon poly5;
    poly5 << QPoint(300, 400);
    poly5 << QPoint(500, 400);
    poly5 << QPoint(800, 500);
    poly5 << QPoint(10, 500);

    QPen linepen;
    linepen.setWidth(1);
    linepen.setJoinStyle(Qt::MiterJoin);

    QBrush fillbrush;
    fillbrush.setStyle(Qt::SolidPattern);

    QPainterPath path1;
    linepen.setColor(Qt::red);
    painter.setPen(linepen);
    fillbrush.setColor(Qt::red);
    path1.addPolygon(poly1);
    painter.drawPolygon(poly1);
    painter.fillPath(path1, fillbrush);

    QPainterPath path2;
    linepen.setColor(Qt::green);
    painter.setPen(linepen);
    fillbrush.setColor(Qt::green);
    path2.addPolygon(poly2);
    painter.drawPolygon(poly2);
    painter.fillPath(path2, fillbrush);

    QPainterPath path3;
    linepen.setColor(Qt::red);
    painter.setPen(linepen);
    fillbrush.setColor(Qt::red);
    path3.addPolygon(poly3);
    painter.drawPolygon(poly3);
    painter.fillPath(path3, fillbrush);

    QPainterPath path4;
    linepen.setColor(Qt::yellow);
    painter.setPen(linepen);
    fillbrush.setColor(Qt::yellow);
    path4.addPolygon(poly4);
    painter.drawPolygon(poly4);
    painter.fillPath(path4, fillbrush);

    QPainterPath path5;
    linepen.setColor(Qt::yellow);
    painter.setPen(linepen);
    fillbrush.setColor(Qt::yellow);
    path5.addPolygon(poly5);
    painter.drawPolygon(poly5);
    painter.fillPath(path5, fillbrush);
}
