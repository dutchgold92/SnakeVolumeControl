#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "utils.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->initSnakeView();
    this->initSnakeController();
}

MainWindow::~MainWindow() {
    delete this->snakeController;
    delete this->snake;
    delete ui;
}

void MainWindow::initSnakeView() {
    QGraphicsScene *snakeScene = new QGraphicsScene();
    snakeScene->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

    ui->snakeView->setScene(snakeScene);
    ui->snakeView->addAction(ui->actionSetDirectionUp);
    ui->snakeView->addAction(ui->actionSetDirectionDown);
    ui->snakeView->addAction(ui->actionSetDirectionLeft);
    ui->snakeView->addAction(ui->actionSetDirectionRight);
}

void MainWindow::initSnakeController() {
    this->snake = Snake::Builder().setTailCoords(std::make_pair(10, 10))->setDirection(Snake::right)->setLength(1)->build();
    this->snakeController = new SnakeController(snake, 20, 20);

    connect(snakeController, SIGNAL(snakeAdvanced(Snake*, std::pair<int, int>*)), this, SLOT(drawSnakeView(Snake*, std::pair<int, int>*)), Qt::QueuedConnection);
    connect(snakeController, SIGNAL(snakeAdvanced(Snake*, std::pair<int, int>*)), this, SLOT(setVolumeDisplay(Snake*)), Qt::QueuedConnection);
    connect(snakeController, SIGNAL(gameOver()), this, SLOT(endGame()), Qt::QueuedConnection);
    connect(ui->snakeView, SIGNAL(snakeViewResized()), this, SLOT(resizeSnakeView()), Qt::QueuedConnection);
    QtConcurrent::run(snakeController, &SnakeController::advance, this->snake);
}

void MainWindow::drawSnakeView(Snake *snake, std::pair<int, int> *foodCoords) {
    ui->snakeView->scene()->clear();
    ui->snakeView->scene()->setSceneRect(0, 0, ui->snakeView->visibleRegion().boundingRect().width(), ui->snakeView->visibleRegion().boundingRect().height());

    Snake::SnakeNode *node = snake->getTail();

    while (node) {
        int x = node->getCoords()->first;
        int y = node->getCoords()->second;

        ui->snakeView->scene()->addRect(offsetByViewSizeX(x, this->snakeController->getXBoundary()), offsetByViewSizeY(y, this->snakeController->getYBoundary()), scaleToViewSizeX(this->snakeController->getXBoundary()), scaleToViewSizeY(this->snakeController->getYBoundary()), QPen(Qt::white), QBrush(Qt::white, Qt::SolidPattern));

        node = node->getNext();
    }

    ui->snakeView->scene()->addEllipse(offsetByViewSizeX(foodCoords->first, this->snakeController->getXBoundary()), offsetByViewSizeY(foodCoords->second, this->snakeController->getYBoundary()), scaleToViewSizeX(this->snakeController->getXBoundary()), scaleToViewSizeY(this->snakeController->getYBoundary()), QPen(Qt::cyan), QBrush(Qt::cyan, Qt::SolidPattern));
}

void MainWindow::endGame() {
    ui->snakeView->scene()->setBackgroundBrush(QBrush(Qt::red, Qt::SolidPattern));
}

unsigned int MainWindow::offsetByViewSizeX(int xCoord, int xBoundary) {
    return (xCoord * scaleToViewSizeX(xBoundary));
}

unsigned int MainWindow::offsetByViewSizeY(int yCoord, int yBoundary) {
    return (yCoord * scaleToViewSizeY(yBoundary));
}

unsigned int MainWindow::scaleToViewSizeX(int xBoundary) {
    return (ui->snakeView->sceneRect().width() / xBoundary);
}

unsigned int MainWindow::scaleToViewSizeY(int yBoundary) {
    return (ui->snakeView->sceneRect().height() / yBoundary);
}

void MainWindow::doPause() {
    if (this->snakeController->isAlive()) {
        if (this->snakeController->togglePause()) {
            ui->pauseButton->setText(LABEL_PAUSE_BUTTON_RESUME);
        } else {
            ui->pauseButton->setText(LABEL_PAUSE_BUTTON_PAUSE);
        }
    }
}

/* FIXME: Setting direction... Needs to also only allow one change per frame, else you can move backwards */
void MainWindow::on_actionSetDirectionUp_triggered() {
    if (this->snake->getDirection() != Snake::down) {
        this->snake->setDirection(Snake::up);
    }
}

void MainWindow::on_actionSetDirectionDown_triggered() {
    if (this->snake->getDirection() != Snake::up) {
        this->snake->setDirection(Snake::down);
    }
}

void MainWindow::on_actionSetDirectionLeft_triggered() {
    if (this->snake->getDirection() != Snake::right) {
        this->snake->setDirection(Snake::left);
    }
}

void MainWindow::on_actionSetDirectionRight_triggered() {
    if (this->snake->getDirection() != Snake::left) {
        this->snake->setDirection(Snake::right);
    }
}

void MainWindow::setVolumeDisplay(Snake *snake) {
    ui->volumeDisplay->setValue(snake->getSize());
}

void MainWindow::on_pauseButton_clicked() {
    doPause();
}

void MainWindow::on_actionPause_triggered() {
    doPause();
}

void MainWindow::resizeSnakeView() {
    this->drawSnakeView(this->snake, this->snakeController->getFoodCoords());
}

void MainWindow::on_resetButton_clicked() {
    this->snakeController->stop();
    delete this->snakeController;
    delete this->snake;

    this->initSnakeView();
    this->initSnakeController();
}
