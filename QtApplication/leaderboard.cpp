#include "leaderboard.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <algorithm>

leaderBoard::leaderBoard(QString filename, QWidget *parent)
    : QWidget(parent), m_filename(filename)
{
    QFile file(m_filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "on lit";
        QTextStream in(&file);
        QString content = in.readAll();
        QStringList lines = content.split("\n");

        for (const QString& line : lines) {
            QStringList parts = line.split(", ");
            if (parts.size() == 2) {
                QString username = parts[0].trimmed();
                int score = parts[1].toInt();
                qDebug() << username << score;
                m_users.append(qMakePair(username, score));
            }
        }
        file.close();
    }
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *leaderBoard = new QLabel("Classement");
    QPushButton *menuButton = new QPushButton("quitter");
    QLabel *position1 = new QLabel("1er");
    QLabel *position2 = new QLabel("2e");
    QLabel *position3 = new QLabel("3e");
    QLabel *position4 = new QLabel("4e");
    QLabel *position5 = new QLabel("5e");
    QLabel *position6 = new QLabel("6e");
    QLabel *position7 = new QLabel("7e");
    QLabel *position8 = new QLabel("8e");
    QLabel *position9 = new QLabel("9e");
    QLabel *position10 = new QLabel("10e");
    joueur1 = new QLabel("joueur");
    joueur2 = new QLabel("joueur");
    joueur3 = new QLabel("joueur");
    joueur4 = new QLabel("joueur");
    joueur5 = new QLabel("joueur");
    joueur6 = new QLabel("joueur");
    joueur7 = new QLabel("joueur");
    joueur8 = new QLabel("joueur");
    joueur9 = new QLabel("joueur");
    joueur10 = new QLabel("joueur");
    score1 = new QLabel("score");
    score2 = new QLabel("score");
    score3 = new QLabel("score");
    score4 = new QLabel("score");
    score5 = new QLabel("score");
    score6 = new QLabel("score");
    score7 = new QLabel("score");
    score8 = new QLabel("score");
    score9 = new QLabel("score");
    score10 = new QLabel("score");
    QHBoxLayout *layout1 = new QHBoxLayout;
    QHBoxLayout *layout2 = new QHBoxLayout;
    QHBoxLayout *layout3 = new QHBoxLayout;
    QHBoxLayout *layout4 = new QHBoxLayout;
    QHBoxLayout *layout5 = new QHBoxLayout;
    QHBoxLayout *layout6 = new QHBoxLayout;
    QHBoxLayout *layout7 = new QHBoxLayout;
    QHBoxLayout *layout8 = new QHBoxLayout;
    QHBoxLayout *layout9 = new QHBoxLayout;
    QHBoxLayout *layout10 = new QHBoxLayout;
    layout1->addWidget(position1);
    layout1->addWidget(joueur1);
    layout1->addWidget(score1);
    layout2->addWidget(position2);
    layout2->addWidget(joueur2);
    layout2->addWidget(score2);
    layout3->addWidget(position3);
    layout3->addWidget(joueur3);
    layout3->addWidget(score3);
    layout4->addWidget(position4);
    layout4->addWidget(joueur4);
    layout4->addWidget(score4);
    layout5->addWidget(position5);
    layout5->addWidget(joueur5);
    layout5->addWidget(score5);
    layout6->addWidget(position6);
    layout6->addWidget(joueur6);
    layout6->addWidget(score6);
    layout7->addWidget(position7);
    layout7->addWidget(joueur7);
    layout7->addWidget(score7);
    layout8->addWidget(position8);
    layout8->addWidget(joueur8);
    layout8->addWidget(score8);
    layout9->addWidget(position9);
    layout9->addWidget(joueur9);
    layout9->addWidget(score9);
    layout10->addWidget(position10);
    layout10->addWidget(joueur10);
    layout10->addWidget(score10);
    mainLayout->addStretch();
    mainLayout->addWidget(leaderBoard, 0, Qt::AlignCenter);
    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout2);
    mainLayout->addLayout(layout3);
    mainLayout->addLayout(layout4);
    mainLayout->addLayout(layout5);
    mainLayout->addLayout(layout6);
    mainLayout->addLayout(layout7);
    mainLayout->addLayout(layout8);
    mainLayout->addLayout(layout9);
    mainLayout->addLayout(layout10);
    mainLayout->addStretch();
    mainLayout->addWidget(menuButton, 0, Qt::AlignCenter);
    connect(menuButton, &QPushButton::clicked, this, &leaderBoard::menuButtonClicked);
}

void leaderBoard::saveScore(QString username, int score)
{
    bool userFound = false;
    for (int i = 0; i < m_users.size(); ++i) {
        if (m_users[i].first == username) {
            if (score > m_users[i].second) {
                m_users[i].second = score;
            }
            userFound = true; //si l'utilisateur existe
            break;
        }
    }

    if (!userFound) {
        m_users.append(qMakePair(username, score)); //sinon on l'ajoute
    }

    sortScores();
    writeScoresInFile();
}

int leaderBoard::loadScore(QString username)
{
    for (const QPair<QString, int>& user : m_users) {
        if (user.first == username) {
            return user.second;
        }
    }
    return -1;
}

void leaderBoard::load10BestPlayers()
{
    QLabel* joueurs[10] = { joueur1, joueur2, joueur3, joueur4, joueur5, joueur6, joueur7, joueur8, joueur9, joueur10 };
    QLabel* scores[10] = { score1, score2, score3, score4, score5, score6, score7, score8, score9, score10 };

    for (int i = 0; i < 10; ++i)
    {
        if (i < m_users.size())
        {
            joueurs[i]->setText(m_users[i].first);
            scores[i]->setText(QString::number(m_users[i].second));
        }
        else //s'il y a moins que 10 joueurs
        {
            joueurs[i]->setText("-");
            scores[i]->setText("-");
        }
    }
}

void leaderBoard::sortScores() //tri en ordre decroissant
{
    std::sort(m_users.begin(), m_users.end(), [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
        return a.second > b.second;
    });
}

void leaderBoard::writeScoresInFile()
{
    QFile file(m_filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "erreur, le fichier n'a pas pu etre ouvert";
        return;
    }

    QTextStream out(&file);
    for (const QPair<QString, int>& user : m_users) {
        out << user.first << ", " << user.second << "\n";
    }

    file.close();
}

void leaderBoard::menuButtonClicked()
{
    emit goBackToMenu();
}

