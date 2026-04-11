#include "compte.h"

compte::compte(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    usernameEdit = new QLineEdit;
    QLabel* usernameLabel = new QLabel("Username: ");
    QLabel* compteLabel = new QLabel("Compte");
    QPushButton* loginButton = new QPushButton("start");
    mainLayout->addStretch();
    mainLayout->addWidget(compteLabel, 0, Qt::AlignCenter);
    mainLayout->addWidget(usernameLabel, 0, Qt::AlignCenter);
    mainLayout->addWidget(usernameEdit, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(loginButton, 0, Qt::AlignCenter);

    connect(loginButton, &QPushButton::clicked, this, &compte::checkUsername);
}

void compte::checkUsername()
{
    username = usernameEdit->text();
    emit newUserDetected(username);
    emit loginSig();
    usernameEdit->clear();
}

QString compte::getCurrentUsername()
{
    qDebug() << username;
    return username;
}
