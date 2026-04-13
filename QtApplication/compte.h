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
#include <QPainter>
#include <QResizeEvent>

class compte : public QWidget
{
    Q_OBJECT
public:
    compte(QWidget *parent = nullptr);
    QString getCurrentUsername();

private:
    QLineEdit* usernameEdit;
    QLabel* usernameLabel;
    QPushButton* invisibleButton;
    QPushButton* cancelButton;
    QPushButton* startButton;
    QString username;
    QPixmap m_background;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;

public slots:
    void checkUsername();
    void onCancel();

signals:
    void newUserDetected(QString username = "");
    void loginSig();
};
#endif // COMPTE_H
