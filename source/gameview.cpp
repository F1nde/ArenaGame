#include <QGraphicsView>
#include <QMouseEvent>
#include <QListWidget>
#include <QDebug>

#include "gameview.h"

GameView::GameView(QWidget *parent) :
    QGraphicsView(parent)
{
    // Set cursor for the GameView area
    setCursor(QCursor(QPixmap((":\\images\\cursorSword_silver.png"))));
    setAcceptDrops(true);
    playerTurn_ = true;
}

GameView::~GameView()
{
}

void GameView::setEngine(Engine *engine)
{
    engine_ = engine;
}

void GameView::mousePressEvent(QMouseEvent *event)
{
    if(playerTurn_)
    {
        playerTurn_ = false;

        if(event->button() == Qt::RightButton)
        {
            // When mouse is pressed, send information to engine
            engine_->countCordinates(event->x(), event->y(), "right");
        }

        else if(event->button() == Qt::LeftButton)
        {
            // When mouse is pressed, send information to engine
            engine_->countCordinates(event->x(), event->y(), "left");
        }

        playerTurn_ = true;
    }
}

void GameView::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void GameView::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void GameView::dropEvent(QDropEvent *event)
{
    // Get the source widget (in this case, QListWidget)
    QObject *source = event->source();
    QListWidget *widget = dynamic_cast<QListWidget *>(source);

    // If widget was something else than QListWidget, cast fails.
    // Ignore events from unknown sources.
    if (widget != nullptr) {
        QPoint pos = event->pos();
        unsigned int slot = widget->currentRow();
        engine_->countCordinates(pos.x(), pos.y(), slot);
    }
    else {
        event->ignore();
    }
}

void GameView::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    // When shown on screen, stretch the scene to fill the view
    fitInView(this->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
}
