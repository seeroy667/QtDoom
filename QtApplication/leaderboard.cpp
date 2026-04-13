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
    setMinimumSize(1000, 700);


    m_table = new QTableWidget(10, 2, this); // 10 lignes, 2 colonnes
    m_table->setShowGrid(false);
    m_table->verticalHeader()->setVisible(false);
    m_table->horizontalHeader()->setVisible(false);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionMode(QAbstractItemView::NoSelection);
    m_table->setFocusPolicy(Qt::NoFocus);

    // Transparent pour voir l'image en dessous
    m_table->setStyleSheet(
        "QTableWidget { background: transparent; border: none; color: #c8a050; font-size: 16px; font-weight: bold; }"
        "QTableWidget::item { background: transparent; padding: 2px 10px; }"
        );

    // Largeur des colonnes
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    m_table->horizontalHeader()->resizeSection(1, 120);

    // Hauteur des rangées
    for (int i = 0; i < 10; i++)
        m_table->setRowHeight(i, 35);

    QPushButton* menuButton = new QPushButton();
    menuButton->setMinimumSize(450,120);
    menuButton->setStyleSheet(
        "QPushButton {"
        "   border-image: url(:/ressources/quitter1.png) 0 0 0 0 stretch stretch;"
        "}"
        "QPushButton:hover {"
        "   border-image: url(:/ressources/quitter2.png) 0 0 0 0 stretch stretch;"
        "}"
        );
    menuButton->move(450, 640);

    connect(menuButton, &QPushButton::clicked, this, &leaderBoard::menuButtonClicked);
    m_background.load(":/ressources/backgroundClassement.png");

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
    for (int i = 0; i < 9; i++)
    {
        QString nom   = i < m_users.size() ? m_users[i].first  : "-";
        QString score = i < m_users.size() ? QString::number(m_users[i].second) : "-";

        QTableWidgetItem* nomItem   = new QTableWidgetItem(nom);
        QTableWidgetItem* scoreItem = new QTableWidgetItem(score);

        scoreItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        nomItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        nomItem->setForeground(QColor("#c8a050"));
        scoreItem->setForeground(QColor("#c8a050"));

        m_table->setItem(i, 0, nomItem);
        m_table->setItem(i, 1, scoreItem);
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

void leaderBoard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), m_background);
    QWidget::paintEvent(event);
}

void leaderBoard::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    float scaleX = (float)width()  / 1340.0f;
    float scaleY = (float)height() / 860.0f;

    // Ces 4 valeurs définissent le rectangle du tableau dans l'image
    int tableX = (int)(430 * scaleX);
    int tableY = (int)(385 * scaleY);
    int tableW = (int)(480 * scaleX);
    int tableH = (int)(500 * scaleY);

    m_table->setGeometry(tableX, tableY, tableW, tableH);
}

