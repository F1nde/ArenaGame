#include <QDebug>

#include "character.h"

Character::Character(QPixmap image, bool block):
    Entity(image, block)
{
    Q_UNUSED(image);
    Q_UNUSED(block);

    charStats_.alive_ = true;
}

Character::~Character()
{
}

int Character::getHealth()
{
    return charStats_.health_;
}

int Character::getMaxHealth()
{
    return charStats_.maxHealth_;
}

int Character::getArmor()
{
    return charStats_.armor_;
}

int Character::getAttack()
{
    return charStats_.attack_;
}

int Character::getLuck()
{
    return charStats_.luck_;
}

int Character::getMovementPoints()
{
    return charStats_.movementPoints_;
}

int Character::getAttackRange()
{
    return charStats_.range_;
}

int Character::getSpeed()
{
    return charStats_.speed_;
}

std::pair<int,int> Character::takeDamage(int damage)
{
    // Calculate damage taken
    damage = damage - charStats_.armor_;

    // armor can't block all damage
    if(damage <= 0){damage = 1;}

    charStats_.health_ = charStats_.health_ - damage;
    if(charStats_.health_ < 0){charStats_.health_ = 0;}

    qDebug() << charStats_.name_ + " took " + QString::number(damage) + " damage. Health remaining: " + QString::number(charStats_.health_);

    if(charStats_.health_ <= 0)
    {
        charStats_.alive_ = false;
        qDebug() << charStats_.name_ + " died";
    }

    return std::make_pair(damage,charStats_.health_);
}

bool Character::status()
{
    if(charStats_.alive_)
        return true;
    else
        return false;
}

QString Character::getName()
{
    return charStats_.name_;
}

void Character::hitTarget(){}
void Character::randomizeStats(int pool){Q_UNUSED(pool)}
