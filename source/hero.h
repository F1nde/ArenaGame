#ifndef HERO_H
#define HERO_H

#include "character.h"
#include "enemy.h"
#include "item.h"
#include "equipment.h"
#include "consumable.h"

class Hero : public Character
{
public:
    Hero(QPixmap image, bool block, int stats);
    ~Hero();

    // Overwritten hitTarget method
    bool hitTarget(Enemy* target, std::pair<int,int>& info);
    // Overwritten randomizeStats method
    void randomizeStats(int pool);

    // Change current equipment
    Item* changeEquipment(Equipment* item);
    // Use Consumable item
    void consumeItem(Consumable* item);

    // Overwritten stat getters to take item buffs into account
    int getHealth();
    int getMaxHealth();
    int getArmor();
    int getAttack();
    int getLuck();
    int getAttackRange();
    int getMovementPoints();
    int getSpeed();

    // Make hero alive again
    void reviveHero();

private:
    // Apply effects of the given buff to character stats permanently
    // (used with consumables)
    void applyPermanentBuff(Item::Buff buff);
    // Apply effects of the given buff to character stats
    void applyBuff(Item::Buff buff);

    Equipment* equippedItem_;
    Item::Buff currentBuff_;
};

#endif // HERO_H
