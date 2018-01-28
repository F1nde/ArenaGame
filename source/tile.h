#ifndef TILE_H
#define TILE_H

#include <memory>
#include <QPixmap>

#include "entity.h"

class Tile
{
public:
    Tile();
    ~Tile();

    // Getters for Tile images
    QPixmap getTerrainImage();
    QPixmap getObjectImage();
    QPixmap getMiscImage();

    // Returns the possible object in the tile
    Entity* getObjectPointer();

    // Setters for Tile objects
    void setTerrain(Entity* terrain);
    void setObject (Entity* object);
    void setMisc(Entity* misc);

    // Getter and setter for active player
    // marker
    QPixmap getSelection();
    void setSelection(QPixmap image);

    // Getter, setter and remover for allowed
    // move marker
    QPixmap getAllowedAction();
    void setAllowedAction(QPixmap image);
    void removeAllowedAction();

    // Checks if there is Entity on the Tile
    bool getBlock();

    // Get information from characters
    int getMovementPoints();
    int getAttackRange();

    // Removes object from the tile
    void removeObject();

private:
    Entity* terrain_;       // Ground
    Entity* object_;        // Entity (wall, item, character etc.)
    Entity* misc_;          // Misc objects (torch etc.)
    QPixmap active_;
    QPixmap allowedAction_;
};

#endif // TILE_H
