#include <QDebug>

#include "hero.h"
#include "helpers.h"

Hero::Hero(QPixmap image, bool block, int stats):
    Character(image, block), equippedItem_(nullptr),
    currentBuff_({0, 0, 0, 0, 0, 0, 0, 0})
{
    image_ = image;
    block_ = block;

    randomizeStats(stats);
}

Hero::~Hero()
{
    // If Hero was wielding some equipment, delete it
    if (equippedItem_ != nullptr)
        delete equippedItem_;
}

bool Hero::hitTarget(Enemy* target, std::pair<int,int>& info)
{
    bool critical = false;

    // Calculate damage here
    double rawdamage = sqrt(static_cast<double>(charStats_.attack_));
    int damage = (charStats_.speed_ / 2) + (charStats_.attack_ / 2) + static_cast<int>(rawdamage);

    if(charStats_.luck_ > 0)
    {
        int luck = randInt(0,100);

        if(luck < charStats_.luck_)
        {
            damage = damage * 2;
            critical = true;
        }
    }

    info = target->takeDamage(damage + currentBuff_.attack);
    return critical;
}

void Hero::randomizeStats(int pool)
{
    std::vector<int>statBoosts (5,0);
    int stat;

    for(int i = 0; i < 5; ++i)
    {
        stat = randInt(0,pool);
        pool = pool - stat;
        statBoosts[i] = stat;
    }

    charStats_.health_ = 20 + statBoosts[0];
    charStats_.maxHealth_ = charStats_.health_;
    charStats_.attack_ = 2 + statBoosts[1];
    charStats_.speed_ = 0 + statBoosts[2];
    charStats_.armor_ = 0;
    charStats_.luck_ = 0;

    charStats_.movementPoints_ = 4;
    charStats_.range_ = 3;

    // NOTE: Name could be also randomized in future versions
    charStats_.name_ = "HERO";
}

Item* Hero::changeEquipment(Equipment *item)
{
    Equipment *temp = nullptr;

    if (item != nullptr) {
        // If Hero was already wielding equipment, return it
        if (equippedItem_ != nullptr) {
            temp = equippedItem_;
        }
        equippedItem_ = item;

        // Set current buff to item's buff
        applyBuff(item->getBuff());
    }
    return temp;
}

void Hero::consumeItem(Consumable *item)
{
    // Apply item's buff permanently
    applyPermanentBuff(item->getBuff());
    delete item;
}

int Hero::getHealth()
{
    return charStats_.health_ + currentBuff_.health;
}

int Hero::getMaxHealth()
{
    return charStats_.maxHealth_ + currentBuff_.maxHealth;
}

int Hero::getArmor()
{
    return charStats_.armor_ + currentBuff_.armor;
}

int Hero::getAttack()
{
    return charStats_.attack_ + currentBuff_.attack;
}

int Hero::getLuck()
{
    return charStats_.luck_ + currentBuff_.luck;
}

int Hero::getAttackRange()
{
    return charStats_.range_ + currentBuff_.range;
}

int Hero::getMovementPoints()
{
    return charStats_.movementPoints_ + currentBuff_.movementPoints;
}

int Hero::getSpeed()
{
    return charStats_.speed_ + currentBuff_.speed;
}

void Hero::applyPermanentBuff(Item::Buff buff)
{
    charStats_.health_ += buff.health;
    charStats_.maxHealth_ += buff.maxHealth;
    charStats_.armor_ += buff.armor;

    charStats_.attack_ += buff.attack;
    charStats_.luck_ += buff.luck;
    charStats_.range_ += buff.range;

    charStats_.movementPoints_ += buff.movementPoints;
    charStats_.speed_ += buff.speed;

    // Make sure health can not be higher than max health
    if (charStats_.health_ > charStats_.maxHealth_)
        charStats_.health_ = charStats_.maxHealth_;
}

void Hero::applyBuff(Item::Buff buff)
{
    currentBuff_ = buff;
}

void Hero::reviveHero()
{
    charStats_.health_ = 1;
    charStats_.alive_ = true;
}
