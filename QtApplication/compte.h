#ifndef COMPTE_H
#define COMPTE_H

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
#include <QLineEdit>

class compte : public QWidget
{
    Q_OBJECT

public:
    compte(QWidget *parent = nullptr);
    QString getCurrentUsername();

private:
    QLineEdit* usernameEdit;
    QString username;

public slots:
    void checkUsername();

signals:
    void newUserDetected(QString username = ""); //va a leaderboard pour load new user
    void loginSig();
};

#endif // COMPTE_H
