#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QLineEdit>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QStackedWidget>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QList>
#include <QPair>

class leaderBoard : public QWidget
{
    Q_OBJECT

public:
    leaderBoard(QString filename = "", QWidget *parent = nullptr);
    void saveScore(QString username, int score);
    int loadScore(QString username);

private:
    QString m_filename;
    void sortScores();
    void writeScoresInFile();
    QList<QPair<QString, int>> m_users;
};

#endif // LEADERBOARD_H
