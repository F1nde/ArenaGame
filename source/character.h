#ifndef CHARACTER_H
#define CHARACTER_H

#include "entity.h"

class Character : public Entity
{
public:
    Character(QPixmap image, bool block);
    virtual ~Character();

    // Getters for character stats
    // NOTE: These are inpure virtual functions, Enemy
    // uses the default functionality but the Hero class
    // overwrites these to add buffs to values!
    virtual int getHealth();
    virtual int getMaxHealth();
    virtual int getArmor();
    virtual int getAttack();
    virtual int getLuck();
    virtual int getAttackRange();
    virtual int getMovementPoints();
    virtual int getSpeed();

    // Makes the character take damage,
    // returns the damage taken and current health for logging
    std::pair<int,int> takeDamage(int damage);

    // Inpure function which subclasses overwrite to implement
    // doing damage to other characters
    virtual void hitTarget();
    // Inpure function which subclasses overwrite to randomize
    // character's stats
    virtual void randomizeStats(int pool);

    // Returns status dead/alive
    bool status();
    // Returns character name
    QString getName();

protected:
    struct Stats
    {
        // Protection
        int health_;
        int maxHealth_;
        int armor_;

        // Offence
        int attack_;
        int luck_;
        int range_;

        // Movement
        int movementPoints_;
        int speed_;

        // Status
        bool alive_;

        // Shop
        int price_;

        QString name_;
    };

    Stats charStats_;
};

#endif // CHARACTER_H
