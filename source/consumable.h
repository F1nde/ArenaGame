#ifndef CONSUMABLE_H
#define CONSUMABLE_H

#include <QPixmap>

#include "item.h"

// NOTE: This subclass does not include any extra
// functionality. It was inherited from the Item
// for possible future features.
class Consumable : public Item
{
public:
    Consumable(QPixmap image, bool block, QString name, Buff buff);
    ~Consumable();
};

#endif // CONSUMABLE_H
