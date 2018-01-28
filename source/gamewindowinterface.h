#ifndef GAMEWINDOWINTERFACE_H
#define GAMEWINDOWINTERFACE_H

#include "world.h"

// Interface between Game (model) and MainWindow

class GameWindowInterface
{
public:
    virtual ~GameWindowInterface(){}

// NOTE: Apparently moc (Meta-Object Compiler) ignores signals
// in interfaces. These are here for better documentation and
// clearer interface structure. The signals are connected using
// dynamic_cast.
signals:
    virtual void writeToLog(QString msg) = 0;
    virtual void updateView(std::vector<World::change> changes) = 0;
    virtual void changeMapVisibility(bool visible) = 0;
    virtual void updateInventory() = 0;
    virtual void updateEntityInfoView() = 0;
    virtual void updateGameInfo() = 0;

    virtual void gameEnded(unsigned int battlesWon) = 0;
};

#endif // GAMEWINDOWINTERFACE_H
