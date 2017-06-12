#ifndef SNAKECONTROLLER_H
#define SNAKECONTROLLER_H

#include <algorithm>
#include <vector>
#include <QProcess>
#include <QThread>

#include "snake.h"
#include "volume_controller.h"

namespace SnakeVolumeControl {
    class SnakeController;
}

class SnakeController : public QObject {
    Q_OBJECT
private:
    bool alive;
    bool isPaused;
    int xBoundary;
    int yBoundary;
    std::pair<int, int> *foodCoords;
    SnakeVolumeControl::SystemVolumeControlAdapter *volumeController;
    bool isGameOver(Snake *snake);
    void throwFood(Snake *snake);
    std::vector<std::pair<int, int> > findCoordsOccupiedBySnake(Snake *snake);
    unsigned int getRandom(unsigned int limit);
public:
    SnakeController(Snake *snake, int xBoundary, int yBoundary);
    ~SnakeController();
    int getXBoundary();
    int getYBoundary();
    void advance(Snake *snake);
    void stop();
    bool togglePause();
    bool isAlive();
    std::pair<int, int>* getFoodCoords();
signals:
    void snakeAdvanced(Snake *snake, std::pair<int, int> *foodCoords);
    void gameOver();
};

#endif // SNAKECONTROLLER_H
