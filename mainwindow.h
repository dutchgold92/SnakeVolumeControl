#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtConcurrent/QtConcurrent>
#include <QGraphicsScene>
#include <QMainWindow>
#include <QThread>

#include "snake.h"
#include "snake_controller.h"

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
    unsigned int scaleToViewSizeX(int xBoundary);
    unsigned int scaleToViewSizeY(int yBoundary);
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
};

#endif // MAINWINDOW_H
