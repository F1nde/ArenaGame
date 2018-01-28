#include <algorithm>

#include "inventory.h"
#include "consumable.h"
#include "equipment.h"

Inventory::Inventory(int size, ItemGenerator *itemGenerator):
    maxSize_(size), goldAmount_(500)
{
    // Generate some starting items to the inventory
    addItem(itemGenerator->generateConsumable());
    addItem(itemGenerator->generateConsumable());
}

Inventory::~Inventory()
{
    for(unsigned int i = 0; i < items_.size(); ++i)
    {
        if (items_.at(i) != nullptr)
            delete items_.at(i);
        items_.at(i) = nullptr;
    }
}

std::vector<Item *> Inventory::getCurrentItems()
{

    return items_;
}

Item* Inventory::getItemInSlot(int slot)
{
    if (checkBoundaries(slot)) {
        Item* item = items_.at(slot);
        items_.erase(items_.begin() + slot);
        return item;
    }
    else
        return nullptr;
}

Inventory::InventoryStatus Inventory::addItem(Item* item)
{
    if (inventoryFull())
        return InventoryStatus::FULL;
    else
    {
        items_.push_back(item);
        return InventoryStatus::OK;
    }
}

int Inventory::getSize()
{
    return maxSize_;
}

void Inventory::removeItem(unsigned int item)
{
    if(item+1 <= items_.size() and items_.size())
    {
        items_.erase(items_.begin() + item);

    }
}

unsigned int Inventory::getCurrentGold()
{
    return goldAmount_;
}

void Inventory::addGold(unsigned int amount)
{
    goldAmount_ += amount;
}

void Inventory::removeGold(unsigned int amount)
{
    if (goldAmount_ >= amount)
        goldAmount_ -= amount;
}

bool Inventory::checkBoundaries(int i)
{
    return (i >= 0 && (unsigned int) i < items_.size());
}

bool Inventory::inventoryFull()
{
    if (items_.size() == maxSize_ - 1)
        return true;
    else
        return false;
}
