
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
#include <QPainter>
#include <QTableWidget>
#include <QHeaderView>

    class leaderBoard : public QWidget
{
    Q_OBJECT
public:
    leaderBoard(QString filename = "", QWidget *parent = nullptr);
    void saveScore(QString username, int score);
    int loadScore(QString username);
    void load10BestPlayers();
    void selectQuitterButton();
    void activateSelectedButton();
    void unselectQuitterButton();

private:
    QTableWidget* m_table = nullptr;
    QString m_filename;
    void sortScores();
    void writeScoresInFile();
    QList<QPair<QString, int>> m_users;
    QPixmap m_background;
    QPushButton* m_menuButton = nullptr;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;

public slots:
    void menuButtonClicked();

signals:
    void goBackToMenu();
};
#endif // LEADERBOARD_H

