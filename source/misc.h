#ifndef MISC_H
#define MISC_H

#include <QPixmap>

#include "entity.h"

class Misc : public Entity
{
public:
    Misc(QPixmap image, bool block);
    ~Misc();
};

#endif // MISC_H
