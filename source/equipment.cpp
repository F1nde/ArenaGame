#include "equipment.h"

Equipment::Equipment(QPixmap image, bool block, QString name, Buff buff):
    Item(image, block, name, buff)
{
}

Equipment::~Equipment()
{
}
