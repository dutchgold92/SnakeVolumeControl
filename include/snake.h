#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include <utility>

namespace SnakeVolumeControl {
class Snake;
}

class Snake {
public:
    class SnakeNode {
    private:
        SnakeNode *prev;
        SnakeNode *next;
        std::pair<int, int> *coords;
    public:
        SnakeNode(SnakeNode *prev, SnakeNode *next, std::pair<int, int> *coords);
        ~SnakeNode();
        SnakeNode* getPrev();
        SnakeNode* getNext();
        std::pair<int, int>* getCoords();
        void setPrev(SnakeNode *prev);
        void setNext(SnakeNode *next);
    };

    enum Direction {up, down, left, right};

    class Builder {
    private:
        std::pair<int, int> tailCoords;
        unsigned int length;
        Direction direction;
    public:
        Builder();
        Builder* setTailCoords(const std::pair<int, int> tailCoords);
        Builder* setLength(const unsigned int length);
        Builder* setDirection(const Direction direction);
        Snake *build();
    };
private:
    SnakeNode *head;
    SnakeNode *tail;
    Direction direction;
    Direction directionOnNextFrame;
    bool isGrowing;
    unsigned int size;
public:
    Snake(SnakeNode *head, SnakeNode *tail, const Direction direction);
    ~Snake();
    void advance();
    void grow();
    SnakeNode* getHead();
    SnakeNode* getTail();
    Direction getDirection();
    void setDirection(const Direction);
    unsigned int getSize();
};

#endif // SNAKE_H
