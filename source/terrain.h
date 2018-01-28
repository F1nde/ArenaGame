#ifndef TERRAIN_H
#define TERRAIN_H

#include <QPixmap>

#include "entity.h"

class Terrain : public Entity
{
public:
    Terrain(QPixmap image, bool block);
    ~Terrain();
};

#endif // TERRAIN_H
