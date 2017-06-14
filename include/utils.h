#include <string.h>

#include "include/snake.h"

void printSnake(Snake &snake, std::pair<int, int> size) {
    char grid[size.first][size.second];

    for (int i = 0; i < size.first; i++) {
        memset(grid[i], '#', size.second);
    }

    Snake::SnakeNode *snakeNode = snake.getTail();

    for (unsigned int i = 0; i < 5; i++) {
        std::pair<int, int> *nodeCoords = snakeNode->getCoords();
        grid[nodeCoords->second][nodeCoords->first] = '@';

        snakeNode = snakeNode->getNext();
    }

    for (int i = 0; i < size.first; i++) {
        for (int j = 0; j < size.second; j++) {
            std::cout << grid[i][j];
        }

        std::cout << std::endl;
    }
}
