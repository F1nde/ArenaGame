#include <QDebug>

#include "item.h"
#include "helpers.h"

Item::Item(QPixmap image, bool block, QString name, Buff buff):
    Entity(image, block),
    name_(name), buff_(buff)
{
    createDescription();
    calculatePrice();
}

Item::~Item()
{
}

QString Item::getName()
{
    return name_;
}

QString Item::getDescription()
{
    return description_;
}

int Item::getPrice()
{
    return price_;
}

Item::Buff Item::getBuff()
{
    return buff_;
}

void Item::createDescription()
{
    QString description;

    if (buff_.health > 0)
        description.append(QString(" +" + QString::number(buff_.health) + " health"));
    if (buff_.maxHealth > 0)
        description.append(QString(" +" + QString::number(buff_.maxHealth) + " max health"));
    if (buff_.armor > 0)
        description.append(QString(" +" + QString::number(buff_.armor) + " armor"));
    if (buff_.attack > 0)
        description.append(QString(" +" + QString::number(buff_.attack) + " attack"));
    if (buff_.luck > 0)
        description.append(QString(" +" + QString::number(buff_.luck) + " luck"));
    if (buff_.range > 0)
        description.append(QString(" +" + QString::number(buff_.range) + " range"));
    if (buff_.movementPoints > 0)
        description.append(QString(" +" + QString::number(buff_.movementPoints) + " movement points"));
    if (buff_.speed > 0)
        description.append(QString(" +" + QString::number(buff_.speed) + " speed"));

    if (description.isNull())
        description_ = "No effect";
    else
        description_ = description;
}

void Item::calculatePrice()
{
    // Calculate price from the stats. Also add a small base to make it non-zero.
    price_ = (buff_.health * HEALTH_VALUE) + (buff_.maxHealth * MAXHEALTH_VALUE) +
            (buff_.armor * ARMOR_VALUE) + (buff_.attack * ATTACK_VALUE) +
            (buff_.luck * LUCK_VALUE) + (buff_.range * RANGE_VALUE) +
            (buff_.movementPoints * MOVEMENT_VALUE) + (buff_.speed * SPEED_VALUE) +
            randInt(5, 10);

    // Price is multiplied by a random value
    double randomMultiplier = (double)(randInt(8, 12) / (double)10);
    price_ = price_ * randomMultiplier;
}
