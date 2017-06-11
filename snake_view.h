#ifndef SNAKE_VIEW_H
#define SNAKE_VIEW_H

#include <QGraphicsView>
#include <QResizeEvent>

class SnakeView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SnakeView(QWidget *parent = 0);

signals:
    void snakeViewResized();
public slots:

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // SNAKE_VIEW_H
