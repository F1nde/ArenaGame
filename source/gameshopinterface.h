#ifndef GAMESHOPINTERFACE_H
#define GAMESHOPINTERFACE_H

#include <QPixmap>

#include "inventory.h"

// Interface between Game (model) and Shop window

class GameShopInterface
{
public:
    virtual ~GameShopInterface(){}

    // return vector with images of all players characters
    virtual std::vector<QPixmap> getCharImages() = 0;
    //Gets info of current inventory
    virtual std::vector<Item*> getInventoryInfo() = 0;

    // used to generate items to shop
    virtual Item* getRandomEquipment () = 0;
    virtual Item* getRandomConsumable () = 0;

    //used to add and remove gold from player
    virtual unsigned int getPlayerGold () = 0;
    virtual void removePlayerGold (unsigned int gold) = 0;

    // adds new char to team
    virtual bool addNewCharacter() = 0;

    virtual void addItem(Item* item) = 0;

    virtual void startBattle() = 0;

    virtual void changeActivePlayer(int heroNum) = 0;

    virtual void removeItem(unsigned int gold) = 0;
};

#endif // GAMESHOPINTERFACE_H
