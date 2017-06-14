#include "include/snake.h"

/*
 * SnakeNode
 */

Snake::SnakeNode::SnakeNode(SnakeNode *prev, SnakeNode *next, std::pair<int, int> *coords) {
    this->prev = prev;
    this->next = next;
    this->coords = coords;
}

Snake::SnakeNode::~SnakeNode() {
    delete this->coords;
}

Snake::SnakeNode* Snake::SnakeNode::getPrev() {
    return this->prev;
}

void Snake::SnakeNode::setNext(SnakeNode *next) {
    this->next = next;
}

Snake::SnakeNode* Snake::SnakeNode::getNext() {
    return this->next;
}

void Snake::SnakeNode::setPrev(SnakeNode *prev) {
    this->prev = prev;
}

std::pair<int, int>* Snake::SnakeNode::getCoords() {
    return coords;
}

Snake::Builder::Builder() {

}

Snake::Builder* Snake::Builder::setTailCoords(const std::pair<int, int> tailCoords) {
    this->tailCoords = tailCoords;
    return this;
}

Snake::Builder* Snake::Builder::setLength(const unsigned int length) {
    this->length = length;
    return this;
}

Snake::Builder* Snake::Builder::setDirection(const Direction direction) {
    this->direction = direction;
    return this;
}

Snake* Snake::Builder::build() {
    SnakeNode *tail = new SnakeNode(nullptr, nullptr, new std::pair<int, int>(this->tailCoords.first, this->tailCoords.second));

    SnakeNode *prev = tail;
    std::pair<int, int> currentCoords = std::make_pair(this->tailCoords.first, this->tailCoords.second);

    for (unsigned int i = 1; i < length; i++) {
        switch (this->direction) {
        case up:
            currentCoords.second--;
            break;
        case down:
            currentCoords.second++;
            break;
        case left:
            currentCoords.first--;
            break;
        case right:
            currentCoords.first++;
            break;
        }

        SnakeNode *next = new SnakeNode(prev, nullptr, new std::pair<int, int>(currentCoords));
        prev->setNext(next);
        prev = next;
    }

    return new Snake(prev, tail, this->direction);
}

/*
 * Snake
 */
Snake::Snake(SnakeNode *head, SnakeNode *tail, const Direction direction) : isGrowing(false), size(0) {
    this->head = head;
    this->tail = tail;
    this->direction = direction;

    SnakeNode *node = head;

    while (node) {
        this->size++;
        node = node->getPrev();
    }
}

Snake::~Snake() {
    SnakeNode *node = this->tail;

    while (node) {
        SnakeNode *next = node->getNext();
        delete node;
        node = next;
    }
}

void Snake::advance() {
    this->direction = this->directionOnNextFrame;
    std::pair<int, int> *newHeadCoords = new std::pair<int, int>(this->head->getCoords()->first, this->head->getCoords()->second);

    switch (this->direction) {
    case up:
        newHeadCoords->second--;
        break;
    case down:
        newHeadCoords->second++;
        break;
    case left:
        newHeadCoords->first--;
        break;
    case right:
        newHeadCoords->first++;
        break;
    }

    SnakeNode *newHead = new SnakeNode(this->head, nullptr, newHeadCoords);
    this->head->setNext(newHead);
    this->head = newHead;

    if (!this->isGrowing) {
        SnakeNode *oldTail = this->tail;
        this->tail = oldTail->getNext();
        this->tail->setPrev(nullptr);
        delete oldTail;
    } else {
        this->isGrowing = false;
        this->size++;
    }
}

void Snake::grow() {
    this->isGrowing = true;
}

Snake::SnakeNode* Snake::getHead() {
    return this->head;
}

Snake::SnakeNode* Snake::getTail() {
    return this->tail;
}

Snake::Direction Snake::getDirection() {
    return this->direction;
}

void Snake::setDirection(const Direction direction) {
    // Don't set direction directly; only acknowledge most recent change per frame
    this->directionOnNextFrame = direction;
}

unsigned int Snake::getSize() {
    return this->size;
}
