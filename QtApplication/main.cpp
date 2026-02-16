/*
Author: Donavan Sirois
Date: Febuary 1, 2026
File name: main.cpp
Goal: This program runs the main loop for the second semester project: The Doom Engine.
Modifications:
    Date: Febuary 1, 2026
        Author: Donavan Sirois
        Description: Initial creation of the main.cpp file.
*/

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;

    w.show();

    return app.exec();
}
