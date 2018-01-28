#include <QDebug>
#include <QFile>

#include "game.h"
#include "world.h"
#include "tile.h"
#include "consumable.h"
#include "helpers.h"

World::World(QString fileName, ItemGenerator* itemGenerator):
    itemGenerator_(itemGenerator)
{
    loadFromFile(fileName);

    // Get arena name from the file name
    arenaName_ = fileName.split("_").at(1).split(".").at(0);
    arenaName_[0] = arenaName_[0].toUpper();
    qDebug() << "Arena name: " << arenaName_;

    // Get frames for active player
    QPixmap select(":\\images\\selected.png");
    selected_ = select;

    QPixmap move(":\\images\\allowedMove.png");
    allowedMove_ = move;

    QPixmap attack(":\\images\\allowedAttack.png");
    allowedAttack_ = attack;

    // Initialize value
    activePlayer_ = std::make_pair(0,0);
    playersOnMap_ = 0;
    enemiesOnMap_ = 0;
    critical_ = false;

    // Maximum allowed move cordinates
    yMin_ = 5;
    yMax_ = static_cast<int>(gameMap_.size() - 4);

    xMin_ = 5;
    xMax_ = static_cast<int>(gameMap_[0].size() - 4);
}

World::~World()
{
    for(unsigned int i = 0; i < gameMap_.size(); ++i)
    {
        for(unsigned int j = 0; j < gameMap_.size(); ++j)
        {
            delete gameMap_[i][j];
            gameMap_[i][j] = nullptr;
        }
    }
}

QString World::getName()
{
    return arenaName_;
}

void World::loadFromFile(QString fileName)
{
    QFile file(fileName);
    QPixmap sprites(":\\images\\spritesheet.png");

    if(!file.exists())
    {
        qDebug() << "No such File:" << fileName;
        return;
    }

    // Check if legal file
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "File open success";
        QTextStream data(&file);

        // Randomize map
        int randomNumber = randInt(1,3);
        QString yard = "Yard" + QString::number(randomNumber);
        qDebug() << "Randomed" << yard;

        // Get width and height for vector initialisation
        QString line = data.readLine();

        if(line == "[header]")
        {
            line = data.readLine();
            QString subString = line.right(2);
            int width = subString.toInt();

            line = data.readLine();
            subString = line.right(2);
            int height = subString.toInt();

            std::vector<Tile* > row(width);

            // Initialise gameMap size
            for(int i = 0; i < height; ++i)
            {
                gameMap_.push_back(row);

                for(int j = 0; j < width; ++j)
                {
                    gameMap_[i][j] = new Tile;
                }
            }

            // Parse rest of the file
            int x;
            int y;
            int position_y;
            QString type;
            while(!data.atEnd())
            {
                line = data.readLine();

                if(line == "[layer]")
                {
                    position_y = 0;

                    // Get type of data
                    line = data.readLine();
                    type = line.mid(line.indexOf("=") + 1);

                    // Discard one line ( "data=" )
                    line = data.readLine();

                    // Get all information from layer data
                    while(line != "")
                    {
                        line = data.readLine();

                        // All data is parsed to dataList
                        QStringList dataList = line.split(","); 
                        for(int i = 0; i < dataList.size() - 1; ++i)
                        {
                            // no image needed
                            if(dataList.at(i) == "0")
                            {
                                continue;
                            }

                            // Spawnpoint data
                            if(type == "PlayerSpawnPoint")
                            {
                                x = dataList.at(i).toInt();
                                ++i;

                                if(i < dataList.size())
                                {
                                    y = dataList.at(i).toInt();
                                    playerSpawns_.push_back(std::make_pair(x,y));
                                    continue;
                                }
                                else
                                {
                                    qDebug() << "PlayerSpawnPoint y cordinate is missing";
                                    continue;
                                }
                            }

                            else if(type == "EnemySpawnPoint")
                            {
                                x = dataList.at(i).toInt();
                                ++i;

                                if(i < dataList.size())
                                {
                                    y = dataList.at(i).toInt();
                                    enemySpawns_.push_back(std::make_pair(x,y));
                                    continue;
                                }
                                else
                                {
                                    qDebug() << "EnemySpawnPoint y cordinate is missing";
                                    continue;
                                }
                            }

                            // Entity data
                            QPixmap pic = getImgFromTilemap(dataList.at(i), sprites);
                            if(!pic.isNull())
                            {
                                if(type == "Ground")
                                {
                                    Terrain* entity = new Terrain(pic, true);
                                    gameMap_[position_y][i]->setTerrain(entity);
                                }
                                else if(type == "Trees")
                                {
                                    Terrain* entity = new Terrain(pic, true);
                                    gameMap_[position_y][i]->setObject(entity);
                                }
                                else if(type == "Walls")
                                {
                                    Terrain* entity = new Terrain(pic, true);
                                    gameMap_[position_y][i]->setObject(entity);
                                }
                                else if(type == "Misc")
                                {
                                    Misc* entity = new Misc(pic, true);
                                    gameMap_[position_y][i]->setMisc(entity);
                                }
                                else if(type == yard)
                                {
                                    Terrain* entity = new Terrain(pic, true);
                                    gameMap_[position_y][i]->setObject(entity);
                                }
                                else if(type == "Valuables")
                                {
                                    // 1/3 chance to spawn item on a Tile
                                    int itemChange = randInt(1, 3);
                                    if (itemChange == 3) {
                                        Item* entity = itemGenerator_->generateConsumable();
                                        gameMap_[position_y][i]->setObject(entity);
                                    }
                                }
                            }
                            else
                            {
                                qDebug() << "ERROR: picture is NULL";
                            }
                        }

                        ++position_y;
                    }
                }
            }

            // Set changes to have whole map

            change newchange;

            for(int i = 0; i < height; ++i)
            {
                for(int j = 0; j < width; ++j)
                {
                    changes_.push_back(newchange);
                    changes_[(i*width)+j].x = j * 16;
                    changes_[(i*width)+j].y = i * 16;
                    changes_[(i*width)+j].tile = gameMap_[i][j];
                }
            }    

        }
        else
        {
            qDebug() << "ERROR: 1. line is not HEADER. File format is broken";
        }

        data.flush();
    }
    else
    {
        qDebug() << "ERROR: Couldn't open the file " << fileName;
    }
}

QPixmap World::getImgFromTilemap(QString tileNumber, QPixmap sprites)
{
    int numberInt = tileNumber.toInt();

    // one image is 16 x 16 px.
    // +1 empty pixer
    // one item width = 17
    int itemsInARow = (sprites.width() + 1) / 17;

    // which row the item is in
    int row = 0;

    // which item in the row
    int item = 0;

    // it's the last image in the row
    if ( (numberInt % itemsInARow) == 0 )
    {
        row = ( (numberInt / itemsInARow) - 1 );
        item = itemsInARow;
    }
    else
    {
        row = numberInt / itemsInARow;

        item = numberInt % itemsInARow;
        // The image index starts from 1
        // --item so that the index starts form 0 to make finding the image easyer
        --item;
    }

    QPixmap tileImg = sprites.copy( (item*17), (row*17), 16.0, 16.0);
    return tileImg;
}

std::pair<int, int> World::addCharacter(Character* character, QString type)
{
    int x;
    int y;

    if(type == "player")
    {
        x = playerSpawns_[playersOnMap_].first;
        y = playerSpawns_[playersOnMap_].second;
        ++playersOnMap_;
    }
    else if(type == "enemy")
    {
        x = enemySpawns_[enemiesOnMap_].first;
        y = enemySpawns_[enemiesOnMap_].second;
        ++enemiesOnMap_;
    }
    else
    {
        qDebug() << "addCharacter: No character type defined";
    }

    gameMap_[y][x]->setObject(character);
    std::pair<int, int> cordinates(x,y);

    addChange(cordinates, false);
    return cordinates;
}

Entity* World::getObject(std::pair<int,int> position)
{
    Entity* target = gameMap_[position.second][position.first]->getObjectPointer();
    return target;
}

void World::changeSelectedPlayer(std::pair<int,int> position)
{
    // Set frame to next player
    gameMap_[position.second][position.first]->setSelection(selected_);

    // Add change for removing old activePlayer frame
    addChange(activePlayer_, false);

    // Store new frame position (so it can removed when activePlayer changes)
    activePlayer_ = position;

    // Add change about new frame
    addChange(position, true);
}

void World::removeItem(std::pair<int,int> position)
{
    // NOTE: Changes involving item position is handled when activePlayer moves to item's old position
    gameMap_[position.second][position.first]->removeObject();
}

// Return true if movement was success
bool World::moveUnit(std::pair<int,int> target, std::pair<int,int> activePosition)
{   
    // Check if target position is blocked
    bool canMove = gameMap_[target.second][target.first]->getBlock();

    if(canMove)
    {
        // Check if cordinates are inside map
        if(target.second >= yMin_ and target.first >= xMin_
                and target.second < yMax_ and target.first < xMax_)
        {
            // Check if target is close enough
            bool range = rangeCheck(target, activePosition, "movement");

            if(range)
            {
                // Movement is legal.
                Entity* character = gameMap_[activePosition.second][activePosition.first]->getObjectPointer();
                gameMap_[activePosition.second][activePosition.first]->removeObject();

                gameMap_[target.second][target.first]->setObject(character);

                // NOTE: Changes involving starting position is handled when activePlayer is changed
                // New postion
                addChange(target, false);

                return true;
            }
        }
        else
        {
            qDebug() << "Can't move to target position: target location outside arena";
        }
    }
    else
    {
       qDebug() << "Can't move to target position: target location is blocked";
    }

    return false;
}

bool World::attackTarget(std::pair<int,int> target, std::pair<int,int> activePosition, QString playerType)
{
    Entity* entity = getObject(activePosition);
    Entity* entity2 = getObject(target);

    Hero* player;
    Enemy* enemy;

    // Get right types of pointers
    if(playerType == "hero")
    {
        player = dynamic_cast<Hero*>(entity);
        enemy = dynamic_cast<Enemy*>(entity2);
    }
    else if(playerType == "enemy")
    {
        enemy = dynamic_cast<Enemy*>(entity);
        player = dynamic_cast<Hero*>(entity2);
    }
    else
    {
        qDebug() << "Error: attackTarget: No valid playerType";
    }

    if(enemy != nullptr && player != nullptr)
    {
        bool status;
        if(playerType == "hero")
        {
            critical_ = player->hitTarget(enemy, battleInfo_);
            status = enemy->status();
        }
        else
        {
            critical_ = enemy->hitTarget(player, battleInfo_);
            status = player->status();
        }

        if(!status)
        {
            // Character is dead
            // Remove Character from the map
            gameMap_[target.second][target.first]->removeObject();
            addChange(target, false);
        }

        // Return status of character
        return status;
    }
    else
    {
        qDebug() << "Error: attackTarget: Player or Enemy is not a valid target";
        return true;
    }
}

bool World::rangeCheck(std::pair<int,int> target, std::pair<int,int> activePosition, QString type)
{
    // Ask if character has enought movement points.
    int x_change = abs(activePosition.first - target.first);
    int y_change = abs(activePosition.second - target.second);

    int comparative;
    if(type == "movement")
    {
        comparative = gameMap_[activePosition.second][activePosition.first]->getMovementPoints();
    }
    else if(type == "attack")
    {
        // Get attack range here
        comparative = gameMap_[activePosition.second][activePosition.first]->getAttackRange();
    }

    // Check if movement is legal
    if(comparative >= x_change and comparative >= y_change)
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<World::change> World::getChanges()
{
    std::vector<change> changes = changes_;
    changes_.clear();
    return changes;
}

void World::addChange(std::pair<int,int> position, bool activePlayer)
{
    change newchange;
    changes_.push_back(newchange);
    changes_[changes_.size() - 1].x = position.first*16;
    changes_[changes_.size() - 1].y = position.second*16;
    changes_[changes_.size() - 1].tile = gameMap_[position.second][position.first];

    if(activePlayer)
    {
        changes_[changes_.size() - 1].activePlayer = true;
    }
    else
    {
        changes_[changes_.size() - 1].activePlayer = false;
    }
}

// For adding allowedMovemenArea and allowedAttackArea
void World::addAllowedArea(std::pair<int,int> position)
{
    Entity* player = getObject(position);

    Hero* hero = dynamic_cast<Hero*>(player);
    if(hero != nullptr)
    {       
        QString type = "move";
        int p = hero->getMovementPoints();

        // Add movement and attack area to the player position
        for(int k = 0; k < 2; ++k)
        {
            // i = y-axis j = x-axis
            for(int i = p; i >= (p * -1); --i)
            {
                for(int j = p; j >= (p * -1); --j)
                {
                    // Check if cordinates are inside map
                    if(position.second + i >= yMin_ and position.first + j >= xMin_
                            and position.second + i < yMax_ and position.first + j < xMax_)
                    {
                        Entity* entity = gameMap_[position.second + i][position.first + j]->getObjectPointer();

                        if(type == "move")
                        {
                            // Test if entity is item
                            Item* item = dynamic_cast<Item*>(entity);

                            if(entity == nullptr or item != nullptr)
                            {
                                gameMap_[position.second + i][position.first + j]->setAllowedAction(allowedMove_);
                                addChange(std::make_pair(position.first + j, position.second + i), false);
                                moveArea_.push_back(std::make_pair(position.first + j, position.second + i));
                            }
                        }
                        else if(type == "attack")
                        {
                            Enemy* enemy = dynamic_cast<Enemy*>(entity);

                            if(enemy != nullptr)
                            {
                                gameMap_[position.second + i][position.first + j]->setAllowedAction(allowedAttack_);
                                addChange(std::make_pair(position.first + j, position.second + i), false);
                                attackArea_.push_back(std::make_pair(position.first + j, position.second + i));
                            }
                        }
                    }
                }
            }

            // Set values for attackRange checks
            type = "attack";
            p = hero->getAttackRange();
        }
    }
    else
    {
        qDebug() << "addAllowedArea: no player at the position";
    }
}

void World::removeAllowedArea()
{
    for(unsigned int i = 0; i < moveArea_.size(); ++i)
    {
        gameMap_[moveArea_[i].second][moveArea_[i].first]->removeAllowedAction();
        addChange(std::make_pair(moveArea_[i].first, moveArea_[i].second), false);
    }

    for(unsigned int j = 0; j < attackArea_.size(); ++j)
    {
        gameMap_[attackArea_[j].second][attackArea_[j].first]->removeAllowedAction();
        addChange(std::make_pair(attackArea_[j].first, attackArea_[j].second), false);
    }

    moveArea_.clear();
    attackArea_.clear();
}

bool World::getAttackInfo(std::pair<int,int>& info)
{
    info = battleInfo_;
    return critical_;
}

std::vector<std::pair<int,int> > World::findHeroes(std::pair<int,int> position, int range)
{
    // Find nearby Heroes
    std::vector<std::pair<int,int> > targets;
    std::pair<int,int> target;

    // Find nearby Heroes

    // i = y-axis j = x-axis
    for(int i = range; i >= (range * -1); --i)
    {
        for(int j = range; j >= (range * -1); --j)
        {
            // Check if cordinates are inside map
            if(position.second + i >= yMin_ and position.first + j >= xMin_
                    and position.second + i < yMax_ and position.first + j < xMax_)
            {
                Entity* entity = gameMap_[position.second + i][position.first + j]->getObjectPointer();
                Hero* hero = dynamic_cast<Hero*>(entity);

                if(hero != nullptr)
                {
                    target = std::make_pair(position.first + j, position.second + i);
                    targets.push_back(target);
                }
            }
        }
    }

    return targets;
}
