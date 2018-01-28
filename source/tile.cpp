#include <QDebug>

#include "tile.h"
#include "hero.h"
#include "enemy.h"

Tile::Tile()
{
    terrain_ = NULL;
    object_ = NULL;
    misc_ = NULL;
}

Tile::~Tile()
{
    if(terrain_ != NULL)
    {
        delete terrain_;
        terrain_ = NULL;
    }
    if(object_ != NULL)
    {
        Character* target = dynamic_cast<Character*>(object_);

        if(target == nullptr)
        {
            delete object_;
            object_ = NULL;
        }
    }
    if(misc_ != NULL)
    {
        delete misc_;
        misc_ = NULL;
    }
}

QPixmap Tile::getTerrainImage()
{
    if(terrain_ != NULL)
    {
        return terrain_->getImage();
    }

    return QPixmap();
}

QPixmap Tile::getObjectImage()
{
    if(object_ != NULL)
    {
        return object_->getImage();
    }

    return QPixmap();
}

QPixmap Tile::getMiscImage()
{
    if(misc_ != NULL)
    {
        return misc_->getImage();
    }

    return QPixmap();
}

void Tile::setTerrain(Entity* terrain)
{
    terrain_ = terrain;
}

void Tile::setObject(Entity* object)
{
    object_ = object;
}

void Tile::setMisc(Entity* misc)
{
    misc_ = misc;
}

QPixmap Tile::getSelection()
{
    return active_;
}

QPixmap Tile::getAllowedAction()
{
    return allowedAction_;
}

void Tile::removeAllowedAction()
{
    allowedAction_ = QPixmap();
}

Entity* Tile::getObjectPointer()
{
    if(object_ != NULL)
    {
        return object_;
    }

    return nullptr;
}

void Tile::setSelection(QPixmap image)
{
    active_ = image;
}

void Tile::setAllowedAction(QPixmap image)
{
    allowedAction_ = image;
}

// Returns information if character can move to this Tile
bool Tile::getBlock()
{
    if(object_ == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Tile::getMovementPoints()
{
    Character* target = dynamic_cast<Character*>(object_);

    // Check if conversion worked
    if(target != nullptr)
    {
       return target->getMovementPoints();
    }
    else
    {
        qDebug() << "Error: getMovementPoints()";
        return 0;
    }
}

int Tile::getAttackRange()
{
    Character* target = dynamic_cast<Character*>(object_);

    // Check if conversion worked
    if(target != nullptr)
    {
       return target->getAttackRange();
    }
    else
    {
        qDebug() << "Error: getAttackRange()";
        return 0;
    }
}

void Tile::removeObject()
{
    object_ = NULL;
}
