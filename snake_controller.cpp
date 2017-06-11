#include "snake_controller.h"

SnakeController::SnakeController(Snake *snake, int xBoundary, int yBoundary) : isAlive(true), isPaused(false), xBoundary(xBoundary), yBoundary(yBoundary) {
    try {
        this->volumeController = SnakeVolumeControl::getVolumeControlAdapter();
    } catch (SnakeVolumeControl::VolumeControlAdapterNotAvailableException ex) {
        std::cerr << "ERROR: " << ex.what() << std::endl;
        this->volumeController = NULL;
    }

    this->throwFood(snake);
}

SnakeController::~SnakeController() {
    delete this->volumeController;
    delete this->foodCoords;
}

int SnakeController::getXBoundary() {
    return this->xBoundary;
}

int SnakeController::getYBoundary() {
    return this->yBoundary;
}

void SnakeController::advance(Snake *snake) {
    while (isAlive) {
        if (!this->isPaused) {
            snake->advance();
            emit(snakeAdvanced(snake, this->foodCoords));

            if (isGameOver(snake)) {
                emit(gameOver());
                stop();
            } else {
                if (*snake->getHead()->getCoords() == *this->foodCoords) {
                    snake->grow();
                    throwFood(snake);
                }

                if (this->volumeController) {
                    this->volumeController->setVolume(snake->getSize());
                }

                QThread::msleep(100);
            }
        } else {
            QThread::msleep(100);
        }
    }
}

void SnakeController::stop() {
    this->isAlive = false;
}

void SnakeController::togglePause() {
    this->isPaused = !this->isPaused;
}

bool SnakeController::isGameOver(Snake *snake) {
    int xIdx = snake->getHead()->getCoords()->first;
    int yIdx = snake->getHead()->getCoords()->second;

    if ((xIdx < 0) || (xIdx >= this->xBoundary) || (yIdx < 0) || (yIdx >= this->yBoundary)) {
        return true;
    }

    Snake::SnakeNode *prevNode = snake->getHead()->getPrev();

    while (prevNode) {
        if (*prevNode->getCoords() == *snake->getHead()->getCoords()) {
            return true;
        }

        prevNode = prevNode->getPrev();
    }

    return false;
}

void SnakeController::throwFood(Snake *snake) {
    std::vector<std::pair<int, int> > occupiedCoords = findCoordsOccupiedBySnake(snake);
    std::vector<std::pair<int, int> > unoccupiedCoords = std::vector<std::pair<int, int> >();

    for (int i = 0; i < this->xBoundary; i++) {
        for (int j = 0; j < this->yBoundary; j++) {
            unoccupiedCoords.push_back(std::make_pair(i, j));
        }
    }

    for (std::vector<std::pair<int, int> >::const_iterator occupiedIterator = occupiedCoords.begin(); occupiedIterator != occupiedCoords.end(); occupiedIterator++) {
        std::pair<int, int> occupiedCoordsValue = *occupiedIterator;

        for (std::vector<std::pair<int, int> >::iterator unoccupiedIterator = unoccupiedCoords.begin(); unoccupiedIterator != unoccupiedCoords.end(); unoccupiedIterator++) {
            std::pair<int, int> unoccupiedCoordsValue = *unoccupiedIterator;

            if (occupiedCoordsValue.first == unoccupiedCoordsValue.first && occupiedCoordsValue.second == unoccupiedCoordsValue.second) {
                unoccupiedCoords.erase(unoccupiedIterator);
                break;
            }
        }
    }

    this->foodCoords = new std::pair<int, int>(unoccupiedCoords[getRandom(unoccupiedCoords.size())]);
}

std::vector<std::pair<int, int> > SnakeController::findCoordsOccupiedBySnake(Snake *snake) {
    std::vector<std::pair<int, int> > occupiedCoords = std::vector<std::pair<int, int> >();

    Snake::SnakeNode *node = snake->getTail();

    while (node) {
        occupiedCoords.push_back(*node->getCoords());

        node = node->getNext();
    }

    return occupiedCoords;
}

unsigned int SnakeController::getRandom(unsigned int limit) {
    // TODO: better
    srand(time(NULL));
    return (rand() % limit);
}
