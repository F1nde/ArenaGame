#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>

#include "item.h"
#include "itemgenerator.h"

class Inventory
{
public:
    // Return status OK when the operation was succesful,
    // only possible failure should be when the inventory
    // is full.
    enum InventoryStatus { OK, FULL };

    Inventory(int size, ItemGenerator *itemGenerator);
    ~Inventory();

    // Get list of pointers to all current items
    std::vector<Item*> getCurrentItems();

    // Returns item in a slot and removes that item
    // from the inventory
    Item* getItemInSlot(int slot);
    // Adds new item to the inventory
    InventoryStatus addItem(Item* item);
    // Returns inventory's max size
    int getSize();

    // Removes item from inventory
    void removeItem(unsigned int item);

    // Returns player's current amount of gold
    unsigned int getCurrentGold();
    // Adds gold to the player
    void addGold(unsigned int amount);
    // Removes gold from the player
    void removeGold(unsigned int amount);

private:
    // Checks that the index in within the inventory
    // size boundaries
    bool checkBoundaries(int i);
    // Returns inventory status
    bool inventoryFull();

    // NOTE: (Optimization) Change this to std::list for more optimal
    // removing from the middle
    std::vector<Item*> items_;
    unsigned int maxSize_;
    unsigned int goldAmount_;
};

#endif // INVENTORY_H
