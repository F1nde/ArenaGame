#ifndef ENTITYINFOVIEW_H
#define ENTITYINFOVIEW_H

#include <QFrame>
#include <QLabel>

#include "gameinfointerface.h"

class EntityInfoView : public QFrame
{
    Q_OBJECT

public:
    explicit EntityInfoView(QWidget *parent);
    ~EntityInfoView();
    // Sets pointer to model (Game)
    void setGame(GameInfoInterface *game);
    // Updates the char info screen with the given stats
    void update();

private:
    //pointers to all UI widgets
    QLabel* pic_;

    QLabel* currentHealth_;
    QLabel* maxHealth_;
    QLabel* armor_;

    QLabel* attack_;
    QLabel* luck_;
    QLabel* range_;

    QLabel* movement_;
    QLabel* speed_;

    GameInfoInterface *game_;
};

#endif // ENTITYINFOVIEW_H
