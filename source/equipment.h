#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <QPixmap>

#include "item.h"

// NOTE: This subclass does not include any extra
// functionality. It was inherited from the Item
// for possible future features.
class Equipment : public Item
{
public:
    Equipment(QPixmap image, bool block, QString name, Buff buff);
    ~Equipment();
};

#endif // EQUIPMENT_H
