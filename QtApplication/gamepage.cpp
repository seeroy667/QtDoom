/*
Author: Léanne Héroux
Date: Febuary 24, 2026
File name: gamepage.cpp
Description: Code for the page where the game is played
Modifications:
*/

#include "gamepage.h"

GamePage::GamePage(QGraphicsView *view, QWidget *parent)
    : QWidget(parent)
{
    //score
    scoreLabel = new QLabel("0");
    scoreLabel->setStyleSheet("color: white; font-size: 20px; font-weight: bold;");
    scoreLabel->show();

    mainLayout = new QVBoxLayout(this);
    stackedWidget = new QStackedWidget(this);
    addGameWidget(view);
    addMenuPopupWidget();
    addGameoverWidget();
    menuButtons.append(menu_retryButton);
    menuButtons.append(menu_quitButton);
    menuButtons.append(menu_continueButton);
    overButtons.append(over_retryButton);
    overButtons.append(over_quitButton);
    connectButtons();
    //stackedWidget->setCurrentWidget(gameOverWidget);
    m_pauseBackground.load(":/ressources/pause.jpg");
    m_gameOverBackground.load(":/ressources/backgroundMort.jpg");
}

void GamePage::addGameWidget(QGraphicsView *view)
{
    gameWidget = new QWidget(this);
    centralWidget = new QWidget(this);
    gameLayout = new QVBoxLayout(gameWidget);
    centralLayout = new QVBoxLayout(centralWidget);
    gameLayout->addWidget(centralWidget);
    addBarreEtat();
    addVieProgressBar();
    addBallesNum();
    addBallesProgressBar();
    addPowerUpProgressBar();
    //layout du jeu
    centralLayout->addWidget(view);
    gameLayout->addWidget(centralWidget);
    addLayoutBarreEtat();
    gameLayout->addWidget(barWidget);
    stackedWidget->addWidget(gameWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(0);
    gameLayout->setContentsMargins(0, 0, 0, 0);
    gameLayout->setSpacing(0);

}

void GamePage::addVieProgressBar()
{
    m_barVie = new QProgressBar;
    m_barVie->setFixedHeight(30);
    m_barVie->setFixedWidth(200);
    m_barVie->setTextVisible(false);
    m_barVie->setStyleSheet("QProgressBar{background-color:lightgrey; border: 2px solid black; border-radius: 5px;} QProgressBar::chunk{background-color:green;}");
    m_barVie->setRange(0, 5);
    m_barVie->setValue(5);
}

void GamePage::addPowerUpProgressBar()
{
    labelPowerUp = new QLabel("");
    labelPowerUp->setStyleSheet("color: white; font-size: 18px;");

    m_barPowerUp = new QProgressBar;
    m_barPowerUp->setFixedHeight(30);
    m_barPowerUp->setFixedWidth(200);
    m_barPowerUp->setTextVisible(false);
    m_barPowerUp->setRange(0, 100);
    m_barPowerUp->setValue(0);
    m_barPowerUp->setStyleSheet(
        "QProgressBar { background:#222; border: 2px solid #333; border-radius: 4px; }"
        "QProgressBar::chunk { background-color: #00aaff; }"
        );
}
void GamePage::addBallesProgressBar()
{
    //qDebug("on entre dans la fonction");
    labelBalles = new QLabel("balles:");
    m_barBalles = new QProgressBar;
    m_barBalles->setFixedHeight(30);
    m_barBalles->setFixedWidth(102);
    m_barBalles->setTextVisible(false);
    m_barBalles->setStyleSheet("QProgressBar{background:#222; color: white; border: 2px solid #333; border-radius: 4px;} QProgressBar::chunk{background-color:white; width: 8px; margin-right:2px;}");
    m_barBalles->setRange(0,10);
    m_barBalles->setValue(10); //a modifier
}

void GamePage::addBallesNum()
{
    labelAmo = new QLabel("balles:");
    m_writeAmo = new QLineEdit;
    m_writeAmo->setFixedWidth(40);
    m_writeAmo->setReadOnly(true);
    m_writeAmo->setText(QString::number(10)); //a modifier
}

void GamePage::addBarreEtat()
{
    barWidget = new QWidget;
    barWidget->setStyleSheet("background-color: gray;");
    barWidget->setFixedHeight(140);
}

void GamePage::addLayoutBarreEtat()
{
    barLayout = new QHBoxLayout(barWidget);
    barLayout->setContentsMargins(20, 10, 20, 10);

    // --- Colonne GAUCHE : score ---
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setAlignment(Qt::AlignCenter);
    QLabel* scoreTitle = new QLabel("Score");
    scoreTitle->setStyleSheet("color: white; font-size: 14px;");
    scoreLabel->setStyleSheet("color: white; font-size: 20px; font-weight: bold;");
    leftLayout->addWidget(scoreTitle, 0, Qt::AlignCenter);
    leftLayout->addWidget(scoreLabel, 0, Qt::AlignCenter);

    // --- Colonne CENTRE : vie + powerup ---
    QVBoxLayout* centerLayout = new QVBoxLayout();
    centerLayout->setAlignment(Qt::AlignCenter);
    centerLayout->setSpacing(8);
    centerLayout->addWidget(m_barVie,     0, Qt::AlignCenter);
    centerLayout->addWidget(m_barPowerUp, 0, Qt::AlignCenter);

    // --- Colonne DROITE : balles ---
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setAlignment(Qt::AlignCenter);
    QLabel* ballesTitle = new QLabel("Balles");
    ballesTitle->setStyleSheet("color: white; font-size: 14px;");
    rightLayout->addWidget(ballesTitle,  0, Qt::AlignCenter);
    rightLayout->addWidget(m_barBalles,  0, Qt::AlignCenter);


    QHBoxLayout* equalColumns = new QHBoxLayout();
    equalColumns->addLayout(leftLayout,   1);
    equalColumns->addLayout(centerLayout, 1);
    equalColumns->addLayout(rightLayout,  1);

    barLayout->addLayout(equalColumns);
}
void GamePage::addMenuPopupWidget()
{
    addTitleMenu();
    addTitleMenuLayout();
    buttonLayout = new QVBoxLayout;
    //créer les boutons
    addContinueButtonMenu();
    addRetryButtonMenu();
    addQuitButtonMenu();
    //ajouter layout
    addRetryButtonMenuLayout();
    addQuitButtonMenuLayout();
    addContinueButtonMenuLayout();
    addPageMenuLayout();
    stackedWidget->addWidget(popupWidget);
}

void GamePage::addTitleMenu()
{
    popupWidget = new QWidget(this);
    popupLayout = new QVBoxLayout(popupWidget);

    // Plus de stylesheet ni de QLabel background

    popupTitle = new QLabel();
    QPixmap gamePause(":/ressources/pauseLabel.png");
    QPixmap scaled = gamePause.scaled(1400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    popupTitle->setPixmap(scaled);
    popupTitle->setAlignment(Qt::AlignCenter);
    popupTitle->setStyleSheet("background-color: transparent;");
}

void GamePage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (m_pauseBgLabel)
        m_pauseBgLabel->setGeometry(0, 0, popupWidget->width(), popupWidget->height());
}

void GamePage::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (stackedWidget->currentWidget() == popupWidget)
    {
        painter.drawPixmap(0, 0, width(), height(), m_pauseBackground);
    }
    else if (stackedWidget->currentWidget() == gameOverWidget)
    {
        painter.drawPixmap(0, 0, width(), height(), m_gameOverBackground);
    }

    QWidget::paintEvent(event);
}
void GamePage::addTitleMenuLayout()
{
    popupLayout->addSpacing(20);
    popupLayout->addWidget(popupTitle, 0, Qt::AlignCenter);
    popupLayout->addSpacing(20);
}

void GamePage::addRetryButtonMenu()
{
    menu_retryButton = new QPushButton();
    menu_retryButton->setFixedSize(450, 120);
    menu_retryButton->setAttribute(Qt::WA_Hover);
    menu_retryButton->setStyleSheet(
        "QPushButton {"
        "   border-image: url(:/ressources/recommencer1.png) 0 0 0 0 stretch stretch;"
        "}"
        "QPushButton:hover {"
        "   border-image: url(:/ressources/recommencer2.png) 0 0 0 0 stretch stretch;"
        "}"
        );
}

void GamePage::addContinueButtonMenu()
{
    menu_continueButton = new QPushButton();
    menu_continueButton->setFixedSize(450, 120);
    menu_continueButton->setAttribute(Qt::WA_Hover);
    menu_continueButton->setStyleSheet(
        "QPushButton {"
        "   border-image: url(:/ressources/continuer1.png) 0 0 0 0 stretch stretch;"
        "}"
        "QPushButton:hover {"
        "   border-image: url(:/ressources/continuer2.png) 0 0 0 0 stretch stretch;"
        "}"
        );
}

void GamePage::addQuitButtonMenu()
{
    menu_quitButton = new QPushButton();
    menu_quitButton->setFixedSize(450, 120);
    menu_quitButton->setAttribute(Qt::WA_Hover);
    menu_quitButton->setStyleSheet(
        "QPushButton {"
        "   border-image: url(:/ressources/quitter1.png) 0 0 0 0 stretch stretch;"
        "}"
        "QPushButton:hover {"
        "   border-image: url(:/ressources/quitter2.png) 0 0 0 0 stretch stretch;"
        "}"
        );
}

void GamePage::addRetryButtonMenuLayout()
{
    retryButtonLayout = new QHBoxLayout();
    retryButtonLayout->addStretch();
    retryButtonLayout->addWidget(menu_retryButton);
    retryButtonLayout->addStretch();
}

void GamePage::addContinueButtonMenuLayout()
{
    continueButtonLayout = new QHBoxLayout();
    continueButtonLayout->addStretch();
    continueButtonLayout->addWidget(menu_continueButton);
    continueButtonLayout->addStretch();
}

void GamePage::addQuitButtonMenuLayout()
{
    quitButtonLayout = new QHBoxLayout();
    quitButtonLayout->addStretch();
    quitButtonLayout->addWidget(menu_quitButton);
    quitButtonLayout->addStretch();
}
void GamePage::addPageMenuLayout()
{
    retryButtonLayout->setAlignment(Qt::AlignCenter);
    quitButtonLayout->setAlignment(Qt::AlignCenter);
    continueButtonLayout->setAlignment(Qt::AlignCenter);

    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->addLayout(continueButtonLayout);
    buttonLayout->addSpacing(15);
    buttonLayout->addLayout(retryButtonLayout);
    buttonLayout->addSpacing(15);
    buttonLayout->addLayout(quitButtonLayout);

    QHBoxLayout* centerLayout = new QHBoxLayout();
    centerLayout->addStretch();
    centerLayout->addLayout(buttonLayout);
    centerLayout->addStretch();

    popupLayout->addLayout(centerLayout);
    popupLayout->addStretch();
}

void GamePage::addGameoverWidget()
{
    gameOverWidget = new QWidget(this);
    gameOverLayout = new QVBoxLayout(gameOverWidget);
    addTitleOver();
    addRetryButtonOver();
    addQuitButtonOver();
    //ajouter layout
    addTitleOverLayout();
    addRetryButtonOverLayout();
    addQuitButtonOverLayout();
    addPageOverLayout();
    stackedWidget->addWidget(gameOverWidget);
    mainLayout->addWidget(stackedWidget);
}

void GamePage::addTitleOver()
{
    gameOverTitle = new QLabel();
    QPixmap gameOverImg(":/ressources/gameover.png");
    QPixmap scaled = gameOverImg.scaled(1400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    gameOverTitle->setPixmap(scaled);
    gameOverTitle->setAlignment(Qt::AlignCenter);
    gameOverTitle->setStyleSheet("background-color: transparent;");
}

void GamePage::addRetryButtonOver()
{
    over_retryButton = new QPushButton();
    over_retryButton->setFixedSize(450, 120);
    over_retryButton->setAttribute(Qt::WA_Hover);
    over_retryButton->setStyleSheet(
        "QPushButton {"
        "   border-image: url(:/ressources/recommencer1.png) 0 0 0 0 stretch stretch;"
        "}"
        "QPushButton:hover {"
        "   border-image: url(:/ressources/recommencer2.png) 0 0 0 0 stretch stretch;"
        "}"
        );
}

void GamePage::addQuitButtonOver()
{
    over_quitButton = new QPushButton();
    over_quitButton->setFixedSize(450, 120);
    over_quitButton->setAttribute(Qt::WA_Hover);
    over_quitButton->setStyleSheet(
        "QPushButton {"
        "   border-image: url(:/ressources/quitter1.png) 0 0 0 0 stretch stretch;"
        "}"
        "QPushButton:hover {"
        "   border-image: url(:/ressources/quitter2.png) 0 0 0 0 stretch stretch;"
        "}"
        );
}
void GamePage::addTitleOverLayout()
{
    gameOverLayout->addSpacing(20);
    gameOverLayout->addWidget(gameOverTitle);
    gameOverLayout->addSpacing(20);
    buttonOverLayout = new QVBoxLayout;
}

void GamePage::addRetryButtonOverLayout()
{
    retryOverButtonLayout = new QHBoxLayout();
    retryOverButtonLayout->addStretch();
    retryOverButtonLayout->addWidget(over_retryButton);
    retryOverButtonLayout->addStretch();
}

void GamePage::addQuitButtonOverLayout()
{
    quitOverButtonLayout = new QHBoxLayout();
    quitOverButtonLayout->addStretch();
    quitOverButtonLayout->addWidget(over_quitButton);
    quitOverButtonLayout->addStretch();
}

void GamePage::addPageOverLayout()
{
    buttonOverLayout->addLayout(retryOverButtonLayout);
    buttonOverLayout->addLayout(quitOverButtonLayout);
    gameOverLayout->addLayout(buttonOverLayout);
    gameOverLayout->addStretch();
}

void GamePage::connectButtons()
{
    //connect les boutons
    connect(menu_quitButton, &QPushButton::clicked, this, &GamePage::menu_quitClicked);
    connect(menu_retryButton, &QPushButton::clicked, this, &GamePage::menu_retryClicked);
    connect(over_quitButton, &QPushButton::clicked, this, &GamePage::over_quitClicked);
    connect(over_retryButton, &QPushButton::clicked, this, &GamePage::over_retryClicked);
    connect(menu_continueButton, &QPushButton::clicked, this, &GamePage::menu_continueClicked);
    if (stackedWidget->currentWidget()==gameOverWidget)
    {
        //qDebug() << "active==gameOver";
    }
    if (stackedWidget->currentWidget()==popupWidget)
    {
        //qDebug() << "active==popup";
    }
}

void GamePage::showEvent(QShowEvent *event)
{
}

void GamePage::updateHighlight()
{
    menu_retryButton->setStyleSheet(
        "QPushButton { border-image: url(:/ressources/recommencer1.png) 0 0 0 0 stretch stretch; }"
        "QPushButton:hover { border-image: url(:/ressources/recommencer2.png) 0 0 0 0 stretch stretch; }"
        );
    menu_quitButton->setStyleSheet(
        "QPushButton { border-image: url(:/ressources/quitter1.png) 0 0 0 0 stretch stretch; }"
        "QPushButton:hover { border-image: url(:/ressources/quitter2.png) 0 0 0 0 stretch stretch; }"
        );
    menu_continueButton->setStyleSheet(
        "QPushButton { border-image: url(:/ressources/continuer1.png) 0 0 0 0 stretch stretch; }"
        "QPushButton:hover { border-image: url(:/ressources/continuer2.png) 0 0 0 0 stretch stretch; }"
        );

    for (int i = 0; i < overButtons.size(); i++)
        overButtons[i]->setStyleSheet("background-color: black; color: white;");

    // Highlight manette — popup
    if (stackedWidget->currentWidget() == popupWidget)
    {
        if (menuCurrentIndex == 0)
            menu_retryButton->setStyleSheet(
                "QPushButton { border-image: url(:/ressources/recommencer2.png) 0 0 0 0 stretch stretch; }"
                );
        else if (menuCurrentIndex == 1)
            menu_quitButton->setStyleSheet(
                "QPushButton { border-image: url(:/ressources/quitter2.png) 0 0 0 0 stretch stretch; }"
                );
        else if (menuCurrentIndex == 2)
            menu_continueButton->setStyleSheet(
                "QPushButton { border-image: url(:/ressources/continuer2.png) 0 0 0 0 stretch stretch; }"
                );
    }

    // Highlight manette — game over
    if (stackedWidget->currentWidget() == gameOverWidget)
    {
        // Reset normal
        over_retryButton->setStyleSheet(
            "QPushButton { border-image: url(:/ressources/recommencer1.png) 0 0 0 0 stretch stretch; }"
            "QPushButton:hover { border-image: url(:/ressources/recommencer2.png) 0 0 0 0 stretch stretch; }"
            );
        over_quitButton->setStyleSheet(
            "QPushButton { border-image: url(:/ressources/quitter1.png) 0 0 0 0 stretch stretch; }"
            "QPushButton:hover { border-image: url(:/ressources/quitter2.png) 0 0 0 0 stretch stretch; }"
            );

        // Highlight manette
        if (overCurrentIndex == 0)
            over_retryButton->setStyleSheet(
                "QPushButton { border-image: url(:/ressources/recommencer2.png) 0 0 0 0 stretch stretch; }"
                );
        else if (overCurrentIndex == 1)
            over_quitButton->setStyleSheet(
                "QPushButton { border-image: url(:/ressources/quitter2.png) 0 0 0 0 stretch stretch; }"
                );
    }
}

void GamePage::activateSelectedButton()
{
    //qDebug() << "click selection dans game";
    if (stackedWidget->currentWidget() == popupWidget) {
        if (menuCurrentIndex == 0) {
            menu_retryClicked();
        } else if (menuCurrentIndex == 1) {
            menu_quitClicked();
        } else if (menuCurrentIndex == 2) {
            menu_continueClicked();
        }
    }
    else if (stackedWidget->currentWidget() == gameOverWidget) {
        if (overCurrentIndex == 0) {
            over_retryClicked();
        } else if (overCurrentIndex == 1) {
            over_quitClicked();
        }
    }
}

void GamePage::menu_quitClicked()
{
    //qDebug() << "menu_quitClicked";
    stackedWidget->setCurrentWidget(gameWidget);
    emit menu_quitClickedSig();
}

void GamePage::menu_retryClicked()
{
    //qDebug() << "menu_retryClicked";
    stackedWidget->setCurrentWidget(gameWidget);
    emit menu_retryClickedSig();
}

void GamePage::menu_continueClicked()
{
    //qDebug() << "menu_continueClicked";
    stackedWidget->setCurrentWidget(gameWidget);
    emit menu_continueClickedSig();
}

void GamePage::over_quitClicked()
{
    //qDebug() << "over_quitClicked";
    stackedWidget->setCurrentWidget(gameWidget);
    emit over_quitClickedSig();
}

void GamePage::over_retryClicked()
{
    //qDebug() << "over_retryClicked";
    stackedWidget->setCurrentWidget(gameWidget);
    emit over_retryClickedSig();
}

void GamePage::changeButtons()
{
    //qDebug() << "changeButtons";
    if (stackedWidget->currentWidget() == popupWidget) {
        menuCurrentIndex++;
        if (menuCurrentIndex > 2) {
            menuCurrentIndex = 0;
        }
    }
    else if (stackedWidget->currentWidget() == gameOverWidget) {
        overCurrentIndex++;
        if (overCurrentIndex > 1) {
            overCurrentIndex = 0;
        }
    }
    //qDebug() << "menu current index:" << menuCurrentIndex;
    //qDebug() << "over current index:" << overCurrentIndex;
}

void GamePage::setupNextSelect()
{
    //qDebug() << "setup Next";
    menuCurrentIndex = 2;
    overCurrentIndex = 0;
}

void GamePage::showMenuPopup()
{
    stackedWidget->setCurrentWidget(popupWidget);
    updateHighlight();
}

bool GamePage::gameIsOn()
{
    return stackedWidget->currentWidget()==gameWidget;
}

QLineEdit* GamePage::amoEdit()
{
    return m_writeAmo;
}

void GamePage::updateVie(int vie)
{
    m_barVie->setValue(vie);
}

void GamePage::updateBalles(int balles)
{
    m_barBalles->setValue(balles);
}

void GamePage::gameOver()
{
    stackedWidget->setCurrentWidget(gameOverWidget);
    updateHighlight();
}
void GamePage::updateScore(int value)
{
    scoreLabel->setText(QString::number(value));
}
void GamePage::updatePowerUp(float progress)
{
    m_barPowerUp->setValue((int)(progress * 100));

    // Couleur différente selon l'état
    if (progress >= 1.0f)
    {
        // Pleine charge — clignote en blanc
        m_barPowerUp->setStyleSheet(
            "QProgressBar { background:#222; border: 2px solid #00aaff; border-radius: 4px; }"
            "QProgressBar::chunk { background-color: white; }"
            );
    }
    else
    {
        m_barPowerUp->setStyleSheet(
            "QProgressBar { background:#222; border: 2px solid #333; border-radius: 4px; }"
            "QProgressBar::chunk { background-color: #00aaff; }"
            );
    }
}
