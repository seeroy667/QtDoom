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


