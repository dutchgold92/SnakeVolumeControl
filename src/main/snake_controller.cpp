#include "include/snake_controller.h"

SnakeController::SnakeController(Snake *snake, const int xBoundary, const int yBoundary) : alive(true), isPaused(false), xBoundary(xBoundary), yBoundary(yBoundary) {
    try {
        this->volumeController = SnakeVolumeControl::getVolumeControlAdapter();
    } catch (SnakeVolumeControl::VolumeControlAdapterNotAvailableException ex) {
        std::cerr << "ERROR: " << ex.what() << std::endl;
        this->volumeController = nullptr;
    }

    this->throwFood(snake);
}

SnakeController::~SnakeController() {
    delete this->volumeController;
}

int SnakeController::getXBoundary() {
    return this->xBoundary;
}

int SnakeController::getYBoundary() {
    return this->yBoundary;
}

void SnakeController::advance(Snake *snake) {
    while (this->alive) {
        if (!this->isPaused) {
            snake->advance();
            emit(snakeAdvanced(snake, &this->foodCoords));

            if (isGameOver(snake)) {
                emit(gameOver());
                stop();
            } else {
                if (*snake->getHead()->getCoords() == this->foodCoords) {
                    snake->grow();
                    throwFood(snake);
                }

                if (this->volumeController) {
                     this->volumeController->setVolume(std::min(((uint) 100), snake->getSize()));
                }

                QThread::msleep(CONFIG_FRAME_DELAY_MS);
            }
        } else {
            QThread::msleep(CONFIG_FRAME_DELAY_MS);
        }
    }
}

void SnakeController::stop() {
    this->alive = false;
}

bool SnakeController::togglePause() {
    return (this->isPaused = !this->isPaused);
}

bool SnakeController::isAlive() {
    return this->alive;
}

bool SnakeController::isGameOver(Snake *snake) {
    const auto snakeHeadCoords = snake->getHead()->getCoords();
    const int xIdx = snakeHeadCoords->first;
    const int yIdx = snakeHeadCoords->second;

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
    std::vector<std::pair<int, int> > occupiedCoordsVector = findCoordsOccupiedBySnake(snake);
    auto unoccupiedCoordsVector = std::vector<std::pair<int, int> >();

    for (int i = 0; i < this->xBoundary; i++) {
        for (int j = 0; j < this->yBoundary; j++) {
            unoccupiedCoordsVector.push_back(std::make_pair(i, j));
        }
    }

    for (auto occupiedIterator = occupiedCoordsVector.begin(); occupiedIterator != occupiedCoordsVector.end(); occupiedIterator++) {
        std::pair<int, int> occupiedCoordsValue = *occupiedIterator;

        for (auto unoccupiedIterator = unoccupiedCoordsVector.begin(); unoccupiedIterator != unoccupiedCoordsVector.end(); unoccupiedIterator++) {
            std::pair<int, int> unoccupiedCoordsValue = *unoccupiedIterator;

            if (occupiedCoordsValue.first == unoccupiedCoordsValue.first && occupiedCoordsValue.second == unoccupiedCoordsValue.second) {
                unoccupiedCoordsVector.erase(unoccupiedIterator);
                break;
            }
        }
    }

    this->foodCoords = std::pair<int, int>(unoccupiedCoordsVector[getRandom(unoccupiedCoordsVector.size())]);
}

std::vector<std::pair<int, int> > SnakeController::findCoordsOccupiedBySnake(Snake *snake) {
    auto occupiedCoords = std::vector<std::pair<int, int> >();

    Snake::SnakeNode *node = snake->getTail();

    while (node) {
        occupiedCoords.push_back(*node->getCoords());
        node = node->getNext();
    }

    return occupiedCoords;
}

unsigned int SnakeController::getRandom(unsigned int limit) {
    srand(time(nullptr));
    return (rand() % limit);
}

std::pair<int, int>* SnakeController::getFoodCoords() {
    return &this->foodCoords;
}
