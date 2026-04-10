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
    void load10BestPlayers();

private:
    QLabel *joueur1;
    QLabel *joueur2;
    QLabel *joueur3;
    QLabel *joueur4;
    QLabel *joueur5;
    QLabel *joueur6;
    QLabel *joueur7;
    QLabel *joueur8;
    QLabel *joueur9;
    QLabel *joueur10;
    QLabel *score1;
    QLabel *score2;
    QLabel *score3;
    QLabel *score4;
    QLabel *score5;
    QLabel *score6;
    QLabel *score7;
    QLabel *score8;
    QLabel *score9;
    QLabel *score10;

    QString m_filename;
    void sortScores();
    void writeScoresInFile();
    QList<QPair<QString, int>> m_users;

public slots:
    void menuButtonClicked();

signals:
    void goBackToMenu();
};

#endif // LEADERBOARD_H
