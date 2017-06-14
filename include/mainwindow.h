#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtConcurrent/QtConcurrent>
#include <QGraphicsScene>
#include <QMainWindow>
#include <QThread>

#include "include/config.h"
#include "include/snake.h"
#include "include/snake_controller.h"

#define LABEL_PAUSE_BUTTON_PAUSE "Pause"
#define LABEL_PAUSE_BUTTON_RESUME "Resume"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Snake *snake;
    SnakeController *snakeController;
    void initSnakeView();
    void initSnakeController();
    unsigned int offsetByViewSizeX(int xCoord, int xBoundary);
    unsigned int offsetByViewSizeY(int yCoord, int yBoundary);
    unsigned int scaleToViewSizeX(int xBoundary);
    unsigned int scaleToViewSizeY(int yBoundary);
    void doPause();
private slots:
    void drawSnakeView(Snake *snake, std::pair<int, int> *foodCoords);
    void endGame();
    void on_actionSetDirectionUp_triggered();
    void on_actionSetDirectionDown_triggered();
    void on_actionSetDirectionLeft_triggered();
    void on_actionSetDirectionRight_triggered();
    void setVolumeDisplay(Snake *snake);
    void on_pauseButton_clicked();
    void on_actionPause_triggered();
    void resizeSnakeView();
    void on_resetButton_clicked();
};

#endif // MAINWINDOW_H
