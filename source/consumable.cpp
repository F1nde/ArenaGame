#include "consumable.h"

Consumable::Consumable(QPixmap image, bool block, QString name, Buff buff):
    Item(image, block, name, buff)
{
}

Consumable::~Consumable()
{
}
