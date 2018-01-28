#ifndef WORLD_H
#define WORLD_H

#include <QString>
#include <vector>
#include <memory>

#include "tile.h"
#include "character.h"
#include "entity.h"
#include "terrain.h"
#include "item.h"
#include "itemgenerator.h"
#include "misc.h"

class World
{
public:
    World(QString fileName, ItemGenerator *itemGenerator);
    ~World();

    // Getter for arena name
    QString getName();

    // Add character to map
    std::pair<int,int> addCharacter(Character* character, QString type);

    // Get Entity pointer from target Tile
    Entity* getObject(std::pair<int,int> position);

    // Change active player frames to toher player
    void changeSelectedPlayer(std::pair<int,int> position);

    // Remove item from map
    void removeItem(std::pair<int,int> position);

    // Change position of character
    bool moveUnit(std::pair<int,int> target, std::pair<int,int> activePosition);

    // Attack target hero/enemy
    // Returns status of character
    bool attackTarget(std::pair<int,int> target, std::pair<int,int> activePosition, QString playerType);

    struct change
    {
        int x;
        int y;
        Tile* tile;
        bool activePlayer;
    };

    // Checks if target is in range of player
    // Use this for attack and movement checks
    bool rangeCheck(std::pair<int,int> target, std::pair<int,int> activePosition, QString type);

    // Returns pointers to Tiles which contains changes
    std::vector<change> getChanges();

    // Add movement area to map
    void addAllowedArea(std::pair<int,int> position);

    // Remove movement area from map
    void removeAllowedArea();

    // Information from last battle
    // returns true if character did critical hit
    bool getAttackInfo(std::pair<int,int>& info);

    // Finds position of all hearoes inside area
    std::vector<std::pair<int,int> > findHeroes(std::pair<int,int> position, int range);

private:
    void loadFromFile(QString fileName);
    QPixmap getImgFromTilemap(QString tileNumber, QPixmap sprites);
    void addChange(std::pair<int,int> position, bool activePlayer);

    // Arena's name
    QString arenaName_;

    // Pointer to item generator to generate Consumables to the map
    ItemGenerator* itemGenerator_;

    // Frames for active player.
    QPixmap selected_;

    // Marker for allowed actions
    QPixmap allowedMove_;
    QPixmap allowedAttack_;

    // Changes to worldstate.
    std::vector<change> changes_;

    std::vector<std::pair<int,int> > moveArea_;
    std::vector<std::pair<int,int> > attackArea_;

    // Game map.
    std::vector<std::vector<Tile* > > gameMap_;

    // Spawnpoints
    std::vector<std::pair<int,int> > playerSpawns_;
    std::vector<std::pair<int,int> > enemySpawns_;

    std::pair<int,int> activePlayer_;

    // Number for indexing spawns
    int playersOnMap_;
    int enemiesOnMap_;

    // Last battle info
    std::pair<int,int> battleInfo_;
    bool critical_;

    // Maximum movement codrinates
    int yMin_;
    int yMax_;
    int xMin_;
    int xMax_;
};

#endif // WORLD_H
