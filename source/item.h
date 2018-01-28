#ifndef ITEM_H
#define ITEM_H

#include <QPixmap>

#include "entity.h"

class Item : public Entity
{
public:
    struct Buff {
        int health;
        int maxHealth;
        int armor;
        int attack;
        int luck;
        int range;
        int movementPoints;
        int speed;
    };

    Item(QPixmap image, bool block, QString name, Buff buff);
    ~Item();

    // Item name and description getters
    QString getName();
    QString getDescription();
    int getPrice();

    // Returns item's buff
    Buff getBuff();

private:
    // Stat values in gold, used in price calculation
    static const int HEALTH_VALUE = 10;
    static const int MAXHEALTH_VALUE = 15;
    static const int ARMOR_VALUE = 5;
    static const int ATTACK_VALUE = 15;
    static const int LUCK_VALUE = 5;
    static const int RANGE_VALUE = 15;
    static const int MOVEMENT_VALUE = 10;
    static const int SPEED_VALUE = 5;

    // Create dynamic description according to buff struct
    void createDescription();
    void calculatePrice();

    QString name_;
    QString description_;
    int price_;
    Buff buff_;
};

#endif // ITEM_H
