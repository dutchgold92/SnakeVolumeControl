#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "utils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initSnakeView();

    this->snake = Snake::Builder().setTailCoords(std::make_pair(10, 10))->setDirection(Snake::right)->setLength(1)->build();

    this->snakeController = new SnakeController(snake, 20, 20);

    connect(snakeController, SIGNAL(snakeAdvanced(Snake*, std::pair<int, int>*)), this, SLOT(drawSnakeView(Snake*, std::pair<int, int>*)), Qt::QueuedConnection);
    connect(snakeController, SIGNAL(snakeAdvanced(Snake*, std::pair<int, int>*)), this, SLOT(setVolumeDisplay(Snake*)), Qt::QueuedConnection);
    connect(snakeController, SIGNAL(gameOver()), this, SLOT(endGame()), Qt::QueuedConnection);
    QtConcurrent::run(snakeController, &SnakeController::advance, this->snake);
}

MainWindow::~MainWindow()
{
    delete this->snake;
    delete this->snakeController;
    delete ui;
}

void MainWindow::initSnakeView() {
    QGraphicsScene *snakeScene = new QGraphicsScene();
    snakeScene->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

    // TODO: proper sizing
    snakeScene->setSceneRect(0, 0, 200, 200);

    ui->snakeView->setScene(snakeScene);
    ui->snakeView->addAction(ui->actionSetDirectionUp);
    ui->snakeView->addAction(ui->actionSetDirectionDown);
    ui->snakeView->addAction(ui->actionSetDirectionLeft);
    ui->snakeView->addAction(ui->actionSetDirectionRight);
    //ui->snakeView->setSceneRect(0, 0, ui->snakeView->frameSize().width(), ui->snakeView->frameSize().height());
}

void MainWindow::drawSnakeView(Snake *snake, std::pair<int, int> *foodCoords) {
    ui->snakeView->scene()->clear();

    // TODO: proper sizing
    ui->snakeView->scene()->addRect(0, 0, 200, 200, QPen(Qt::white));

    Snake::SnakeNode *node = snake->getTail();

    while (node) {
        int x = node->getCoords()->first;
        int y = node->getCoords()->second;

        ui->snakeView->scene()->addRect(x * 10, y * 10, scaleToViewSizeX(this->snakeController->getXBoundary()), scaleToViewSizeY(this->snakeController->getYBoundary()), QPen(Qt::white), QBrush(Qt::white, Qt::SolidPattern));

        node = node->getNext();
    }

    ui->snakeView->scene()->addRect(foodCoords->first * 10, foodCoords->second * 10, scaleToViewSizeX(this->snakeController->getXBoundary()), scaleToViewSizeY(this->snakeController->getYBoundary()), QPen(Qt::blue), QBrush(Qt::blue, Qt::SolidPattern));
}

void MainWindow::endGame() {
    ui->snakeView->scene()->setBackgroundBrush(QBrush(Qt::red, Qt::SolidPattern));
}

unsigned int MainWindow::scaleToViewSizeX(int xBoundary) {
    return 10;
    //return (ui->snakeView->sceneRect().width() / xBoundary);
}

unsigned int MainWindow::scaleToViewSizeY(int yBoundary) {
    return 10;
    //return (ui->snakeView->sceneRect().height() / yBoundary);
}

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
    this->snakeController->togglePause();
}

void MainWindow::on_actionPause_triggered() {
    this->snakeController->togglePause();
}
