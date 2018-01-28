#ifndef ENTITY_H
#define ENTITY_H

#include <QPixmap>

class Entity
{
public:
    Entity(QPixmap image, bool block);
    virtual ~Entity();

    // Return the image representing the Entity
    QPixmap getImage();
    // Whether the Entity blocks movement or not
    bool isBlock() const;

protected:
    QPixmap image_;
    bool block_;
};

#endif // ENTITY_H
