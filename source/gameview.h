#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

#include "engine.h"

class GameView : public QGraphicsView
{
    Q_OBJECT

public:
    GameView(QWidget *parent);
    ~GameView();

    // Sets pointer to controller (Engine)
    void setEngine(Engine *engine);

protected:
    // Overwritten mouse events to allow dropping
    // items into tiles
    void mousePressEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void showEvent(QShowEvent *event);

    Engine *engine_;
    bool playerTurn_;
};

#endif // GAMEVIEW_H
