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
        qDebug() << "Reading leaderboard file";

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


    m_table = new QTableWidget(10, 2, this);

    m_table->setShowGrid(false);
    m_table->verticalHeader()->setVisible(false);
    m_table->horizontalHeader()->setVisible(false);


    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionMode(QAbstractItemView::NoSelection);
    m_table->setFocusPolicy(Qt::NoFocus);


    m_table->setStyleSheet(
        "QTableWidget { background: transparent; border: none; color: #c8a050; font-size: 16px; font-weight: bold; }"
        "QTableWidget::item { background: transparent; padding: 2px 10px; }"
        );

    // Column resizing behavior
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch); // name expands
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);   // score fixed
    m_table->horizontalHeader()->resizeSection(1, 120);

    // Menu button (Quit / Back)
    m_menuButton = new QPushButton(this);
    m_menuButton->setMinimumSize(450, 120);

    // Button textures (normal + hover)
    m_menuButton->setStyleSheet(
        "QPushButton {"
        "   border-image: url(:/ressources/quitter1.png) 0 0 0 0 stretch stretch;"
        "}"
        "QPushButton:hover {"
        "   border-image: url(:/ressources/quitter2.png) 0 0 0 0 stretch stretch;"
        "}"
        );

    // Connect button click to signal
    connect(m_menuButton, &QPushButton::clicked, this, &leaderBoard::menuButtonClicked);

    // Load background image
    m_background.load(":/ressources/backgroundClassement.png");
}


// Save or update a player's score
void leaderBoard::saveScore(QString username, int score)
{
    bool userFound = false;

    // Check if user already exists
    for (int i = 0; i < m_users.size(); ++i) {
        if (m_users[i].first == username) {

            // Keep only the best score
            if (score > m_users[i].second) {
                m_users[i].second = score;
            }

            userFound = true;
            break;
        }
    }


    if (!userFound) {
        m_users.append(qMakePair(username, score));
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
    for (int i = 0; i < 10; i++)
    {

        QString nom   = i < m_users.size() ? m_users[i].first  : "-";
        QString score = i < m_users.size() ? QString::number(m_users[i].second) : "-";

        QTableWidgetItem* nomItem   = new QTableWidgetItem(nom);
        QTableWidgetItem* scoreItem = new QTableWidgetItem(score);


        scoreItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        nomItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        // Apply gold color
        nomItem->setForeground(QColor("#c8a050"));
        scoreItem->setForeground(QColor("#c8a050"));


        m_table->setItem(i, 0, nomItem);
        m_table->setItem(i, 1, scoreItem);
    }
}



void leaderBoard::sortScores()
{
    std::sort(m_users.begin(), m_users.end(),
              [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
                  return a.second > b.second;
              });
}



void leaderBoard::writeScoresInFile()
{
    QFile file(m_filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Error: could not open file";
        return;
    }

    QTextStream out(&file);

    // Save format: "username, score"
    for (const QPair<QString, int>& user : m_users) {
        out << user.first << ", " << user.second << "\n";
    }

    file.close();
}



void leaderBoard::menuButtonClicked()
{
    emit goBackToMenu();
    unselectQuitterButton();
}



void leaderBoard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Scale background to window size
    painter.drawPixmap(0, 0, width(), height(), m_background);

    QWidget::paintEvent(event);
}


// Handle window resize and scale UI accordingly
void leaderBoard::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    // Compute scaling factors (relative to design resolution)
    float scaleX = (float)width()  / 1340.0f;
    float scaleY = (float)height() / 860.0f;

    // Position and size table
    int tableX = (int)(430 * scaleX);
    int tableY = (int)(340 * scaleY);
    int tableW = (int)(480 * scaleX);
    int tableH = (int)(500 * scaleY);

    m_table->setGeometry(tableX, tableY, tableW, tableH);

    int rowHeight = (int)(35 * scaleY);
    for (int i = 0; i < 10; i++)
        m_table->setRowHeight(i, rowHeight);


    m_table->horizontalHeader()->resizeSection(1, (int)(120 * scaleX));


    int fontSize = std::max(8, (int)(16 * std::min(scaleX, scaleY)));

    m_table->setStyleSheet(
        QString("QTableWidget { background: transparent; border: none; color: #c8a050; font-size: %1px; font-weight: bold; }"
                "QTableWidget::item { background: transparent; padding: 2px 10px; }")
            .arg(fontSize)
        );

    // Center menu button
    int btnW = (int)(450 * scaleX);
    int btnH = (int)(120 * scaleY);
    int btnX = (width() - btnW) / 2;
    int btnY = (int)(730 * scaleY);

    m_menuButton->setGeometry(btnX, btnY, btnW, btnH);
}


// Force button into "hover/selected" state
void leaderBoard::selectQuitterButton()
{
    m_menuButton->setStyleSheet(
        "QPushButton {border-image: url(:/ressources/quitter2.png) 0 0 0 0 stretch stretch;}"
        );
}


// Trigger selected button (keyboard/controller support)
void leaderBoard::activateSelectedButton()
{
    menuButtonClicked();
}


// Restore default button style
void leaderBoard::unselectQuitterButton()
{
    m_menuButton->setStyleSheet(
        "QPushButton {"
        "   border-image: url(:/ressources/quitter1.png) 0 0 0 0 stretch stretch;"
        "}"
        "QPushButton:hover {"
        "   border-image: url(:/ressources/quitter2.png) 0 0 0 0 stretch stretch;"
        "}"
        );
}
