#ifndef GAMEINFOINTERFACE_H
#define GAMEINFOINTERFACE_H

#include <vector>

#include "item.h"

// Interface between Game (model) and MainWindow widgets
// Basic game information (characters, inventory, items) can be
// fetched and modified via this interface.

class GameInfoInterface
{
public:
    // Struct used to pass game information
    // from Game (model) to MainWindow (view)
    struct GameInfo {
        QString arenaName;
        int battlesWon;
        int gold;
        int heroesAlive;
    };

    //struct with all the needed information of a character
    //show in the information menu ingame
    struct CharStats {

        QPixmap charImage;

        int health;
        int maxHealth;
        int armor;

        int attack;
        int luck;
        int range;

        int movementPoints;
        int speed;

    };

    virtual ~GameInfoInterface(){}

    virtual std::vector<Item*> getInventoryInfo() = 0;
    virtual CharStats getEntityInfo() = 0;
    virtual GameInfo getGameInfo() = 0;



};

#endif // GAMEINFOINTERFACE_H
