#include "entity.h"

Entity::Entity(QPixmap image, bool block):
    image_(image), block_(block)
{
}

Entity::~Entity()
{
}

QPixmap Entity::getImage()
{
    return image_;
}

bool Entity::isBlock() const
{
    return block_;
}
