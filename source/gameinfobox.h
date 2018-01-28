#ifndef GAMEINFOBOX_H
#define GAMEINFOBOX_H

#include <QTabWidget>
#include <QLabel>

#include "gameinfointerface.h"

class GameInfoBox : public QTabWidget
{
    Q_OBJECT

public:
    explicit GameInfoBox(QWidget *parent);
    ~GameInfoBox();

    // Sets pointer to model (Game)
    void setGame(GameInfoInterface *game);
    // Updates the info box with new information
    void update();

private:
    void getLabelPointers();

    // Structs to hold pointers to labels to update
    struct GeneralLabels {
        QLabel* arenaName;
        QLabel* battlesWon;
        QLabel* gold;
        QLabel* heroesAlive;
    };
    struct StatisticsLabels {
        QLabel* someStat;
    };

    GameInfoInterface *game_;
    GeneralLabels generalLabels_;
    StatisticsLabels statLabels_;
};

#endif // GAMEINFOBOX_H
