// compte.h - Ajoutez ces membres si ce n'est pas déjà fait
// QPushButton* invisibleButton;

// compte.cpp
#include "compte.h"

compte::compte(QWidget *parent)
    : QWidget(parent)
{

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    usernameEdit = new QLineEdit(this);
    usernameEdit->setStyleSheet(
        "QLineEdit {"
        "  background: transparent;"
        "  border: 2px solid #00AA00;"
        "  border-radius: 3px;"
        "  color: #00FF41;"
        "  font-family: 'Courier New';"
        "  font-size: 14px;"
        "  padding: 4px 8px;"
        "  selection-background-color: #00FF41;"
        "  selection-color: #000000;"
        "}"
        "QLineEdit:focus {"
        "  border: 2px solid #00FF41;"
        "}"
        );

    invisibleButton = new QPushButton("", this);
    invisibleButton->setStyleSheet(
        "QPushButton {"
        "  background: transparent;"
        "  border: none;"
        "}"
        "QPushButton:hover {"
        "  background: rgba(0, 255, 65, 0.05);"
        "}"
        );
    invisibleButton->setCursor(Qt::PointingHandCursor);


    cancelButton = new QPushButton("", this);
    cancelButton->setStyleSheet(
        "QPushButton { background: transparent; border: none; }"
        );
    cancelButton->setCursor(Qt::PointingHandCursor);


    startButton = new QPushButton("start", this);
    startButton->setStyleSheet("QPushButton { background: transparent; border: none; color: transparent; }");

    connect(invisibleButton, &QPushButton::clicked, this, &compte::checkUsername);
    connect(cancelButton, &QPushButton::clicked, this, &compte::onCancel);
    connect(startButton, &QPushButton::clicked, this, &compte::checkUsername);

    m_background.load(":/ressources/backgroundUser.png");

}

void compte::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    int W = width();
    int H = height();


    int dlgX = (int)(W * 0.315);
    int dlgY = (int)(H * 0.305);
    int dlgW = (int)(W * 0.365);
    int dlgH = (int)(H * 0.340);


    usernameEdit->setGeometry(
        dlgX + (int)(dlgW * 0.075),
        dlgY + (int)(dlgH * 0.45),
        (int)(dlgW * 0.87),
        (int)(dlgH * 0.16)
        );


    int btnY = dlgY + (int)(dlgH * 0.72);
    int btnH = (int)(dlgH * 0.20);


    cancelButton->setGeometry(
        dlgX + (int)(dlgW * 0.06),
        btnY,
        (int)(dlgW * 0.38),
        btnH
        );

    invisibleButton->setGeometry(
        dlgX + (int)(dlgW * 0.50),
        btnY,
        (int)(dlgW * 0.38),
        btnH
        );
}
void compte::onCancel()
{
    usernameEdit->clear();
    if (username!="")
    {
        emit annulerChangeUser();
    }
}

void compte::checkUsername()
{
    username = usernameEdit->text();
    if (!username.isEmpty()) {
        emit newUserDetected(username);
        emit loginSig();
        usernameEdit->clear();
    }
}

QString compte::getCurrentUsername()
{
    return username;
}

void compte::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), m_background);
    QWidget::paintEvent(event);
}

void compte::activateSelectedButton()
{
    invisibleButton->click();
}
