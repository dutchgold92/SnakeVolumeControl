#include "snake_view.h"

SnakeView::SnakeView(QWidget *parent) : QGraphicsView(parent) {

}

void SnakeView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    emit(snakeViewResized());
}
