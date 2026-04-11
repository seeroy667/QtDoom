/*
Author: Léanne Héroux
Date: Febuary 24, 2026
File name: gamepage.h
Description: Code for the page where the game is played
Modifications:
*/

#ifndef GAMEPAGE_H
#define GAMEPAGE_H

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
#include <QPainter>


class GamePage : public QWidget
{
    Q_OBJECT

public:
    explicit GamePage(QGraphicsView *view, QWidget *parent = nullptr);
    bool gameIsOn();
    void updateVie(int vie);
    void updateBalles(int balles);
    void updateScore(int value);
    void updatePowerUp(float progress);

    QLineEdit* amoEdit();

private:
    QStackedWidget *stackedWidget;

    QWidget *gameWidget;
    QWidget *popupWidget;
    QWidget *gameOverWidget;
    QWidget *centralWidget;
    QWidget *barWidget;

    QPushButton *menu_retryButton;
    QPushButton *menu_quitButton;
    QPushButton *menu_continueButton;
    QPushButton *over_retryButton;
    QPushButton *over_quitButton;
    QPushButton *m_shootButton;

    QVBoxLayout *mainLayout;
    QVBoxLayout *gameLayout;
    QVBoxLayout *centralLayout;
    QVBoxLayout *popupLayout;
    QVBoxLayout *buttonLayout;
    QVBoxLayout *buttonOverLayout;
    QVBoxLayout *gameOverLayout;

    QHBoxLayout *barLayout;
    QHBoxLayout *retryButtonLayout;
    QHBoxLayout *quitButtonLayout;
    QHBoxLayout *continueButtonLayout;
    QHBoxLayout *retryOverButtonLayout;
    QHBoxLayout *quitOverButtonLayout;

    QLabel *labelVie;
    QLabel *labelBalles;
    QLabel *labelAmo;
    QLabel *popupTitle;
    QLabel *gameOverTitle;
    QLabel *scoreLabel;

    QFont font;
    QFont overFont;

    QProgressBar *m_barVie;
    QProgressBar *m_barBalles;
    QLineEdit *m_writeAmo;

    QVector<QPushButton*> menuButtons;
    QVector<QPushButton*> overButtons;

    QPixmap m_pauseBackground;
    QLabel* m_pauseBgLabel = nullptr;
    QPixmap m_gameOverBackground;
    QLabel *m_hudBgLabel = nullptr;

    bool potTurns=false;
    bool shootPress=false;
    int menuCurrentIndex=0;
    int overCurrentIndex=0;

    QProgressBar* m_barPowerUp = nullptr;
    QLabel* labelPowerUp = nullptr;


    void addGameWidget(QGraphicsView *view);
    void addMenuPopupWidget();
    void addGameoverWidget();
    void connectButtons();
    void addVieProgressBar();
    void addBallesNum();
    void addBarreEtat();
    void addLayoutBarreEtat();
    void addRetryButtonMenu();
    void addContinueButtonMenu();
    void addQuitButtonMenu();
    void addRetryButtonMenuLayout();
    void addContinueButtonMenuLayout();
    void addQuitButtonMenuLayout();
    void addPageMenuLayout();
    void addTitleMenu();
    void addTitleMenuLayout();
    void addTitleOver();
    void addRetryButtonOver();
    void addQuitButtonOver();
    void addTitleOverLayout();
    void addRetryButtonOverLayout();
    void addQuitButtonOverLayout();
    void addPageOverLayout();
    void addBallesProgressBar();
    void addPowerUpProgressBar();


signals:
    void menu_quitClickedSig();
    void menu_retryClickedSig();
    void menu_continueClickedSig();
    void over_quitClickedSig();
    void over_retryClickedSig();

protected:
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void menu_quitClicked();
    void menu_retryClicked();
    void menu_continueClicked();
    void over_quitClicked();
    void over_retryClicked();

public slots:
    void changeButtons();
    void setupNextSelect();
    void updateHighlight();
    void activateSelectedButton();
    void showMenuPopup();
    void gameOver();
};

#endif // GAMEPAGE_H
