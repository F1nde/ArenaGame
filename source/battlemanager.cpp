#include <QtGlobal>
#include <QDebug>

#include "battlemanager.h"
#include "helpers.h"

BattleManager::BattleManager(World* world, int tier)
{
    world_ = world;

    heroesDied_ = 0;
    enemiesDied_ = 0;

    // Initialize enemies and turnQueue
    createEnemies(tier);
    resetTurnQueue();
}

BattleManager::~BattleManager()
{
    for(unsigned int i = 0; i < enemies_.size(); ++i)
    {
        delete enemies_.at(i);
        enemies_.at(i) = NULL;
    }
}

void BattleManager::createEnemies(int tier)
{
    // Randomize enemy quantity
    int enemies = randInt(1,4);

    // Get stat pool
    int pool = tier * 10;

    // Divide stats and create enemies
    for(int i = 0; i < enemies; ++i)
    {
        Enemy* enemy = new Enemy(getEnemyPicture(), true, pool/enemies, world_);
        enemies_.push_back(enemy);
    }

    // Set enemies to map
    for(unsigned int j = 0; j < enemies_.size(); ++j)
    {
        // Add enemy to game map
        std::pair<int,int> cordinates = world_->addCharacter(enemies_.at(j),"enemy");
        // Save cordinates
        addCharacter(cordinates, "enemy");
    }
}

std::pair<int,int> BattleManager::playersTurn()
{
    return turnQueue_[activePlayer_].position_;
}

// true = next player is hero
// false = next player is enemy
bool BattleManager::changeTurn()
{
    // Skip all dead players
    for(unsigned int i = activePlayer_; i < turnQueue_.size(); ++i)
    {
        ++activePlayer_;

        if(activePlayer_ == turnQueue_.size())
        {
            // Queue ended, create new
            // No more possibility of orphan turns
            resetTurnQueue();
            break;
        }
        else if(turnQueue_[activePlayer_].status_)
        {
            // Found player alive
            break;
        }
    }

    // Check if next player is enemy
    QString type = turnQueue_[activePlayer_].type_;

    if(type == "hero")
    {
        return true;
    }
    else
    {
        return false;
    }
}

void BattleManager::changeActivePlayerPosition(std::pair<int,int> position)
{
    unsigned int index = turnQueue_[activePlayer_].index_;
    turnQueue_[activePlayer_].position_ = position;
    QString type = turnQueue_[activePlayer_].type_;

    if(type == "hero")
    {
        heroPositions_[index] = position;
    }
    else
    {
        enemyPositions_[index] = position;
    }
}

void BattleManager::addCharacter(std::pair<int, int> position, QString type)
{
    if(type == "player")
    {
        heroPositions_.push_back(position);
    }
    else if(type == "enemy")
    {
        enemyPositions_.push_back(position);
    }
    else
    {
        qDebug() << "BattleManager: addCharacter doesn't have a type";
    }
}

bool BattleManager::markCharacterDead(std::pair<int, int> position)
{
    for(unsigned int i = 0; i < turnQueue_.size(); ++i)
    {
        if(turnQueue_[i].position_ == position)
        {
            turnQueue_[i].status_ = false;

            if(turnQueue_[i].type_ == "hero")
            {
                ++heroesDied_;

                if(heroPositions_.size() == heroesDied_)
                {
                    qDebug() << "Enemy won!";
                    return true;
                }
            }
            else
            {
                ++enemiesDied_;

                if(enemyPositions_.size() == enemiesDied_)
                {
                    qDebug() << "Player won!";
                    return true;
                }
            }

            break;
        }
    }
    return false;
}

void BattleManager::removeCharacter(std::pair<int, int> position, QString type)
{
    if(type == "hero")
    {
        for(unsigned int i = 0; i < heroPositions_.size(); ++i)
        {
            if(heroPositions_[i].first == position.first && heroPositions_[i].second == position.second)
            {
                heroPositions_.erase(heroPositions_.begin() + i);
                break;
            }
        }
    }
    else if(type == "enemy")
    {
        for(unsigned int i = 0; i < enemyPositions_.size(); ++i)
        {
            if(enemyPositions_[i].first == position.first && enemyPositions_[i].second == position.second)
            {
                enemyPositions_.erase(enemyPositions_.begin() + i);
                break;
            }
        }
    }
    else
    {
        qDebug() << "BattleManager: removeCharacter doesn't have a type";
    }
}

void BattleManager::resetTurnQueue()
{
    // Remove dead characters
    if(enemiesDied_ + heroesDied_ != 0)
    {
        for(unsigned int k = 0; k < turnQueue_.size(); ++k)
        {
            if(turnQueue_[k].status_ == false)
            {
                removeCharacter(turnQueue_[k].position_, turnQueue_[k].type_);
            }
        }
    }

    turnQueue_.clear();

    Entity* entity;
    Character* character;
    chara_ p;
    p.status_ = true;

    for(unsigned int i = 0; i < heroPositions_.size(); ++i)
    {
        entity = world_->getObject(heroPositions_.at(i));
        character = dynamic_cast<Character*>(entity);

        p.speed_ = character->getSpeed();
        p.position_ = heroPositions_.at(i);
        p.index_ = i;
        p.type_ = "hero";

        addCharacterToQueue(p);
    }

    for(unsigned int j = 0; j < enemyPositions_.size(); ++j)
    {
        entity = world_->getObject(enemyPositions_.at(j));

        character = dynamic_cast<Character*>(entity);

        p.speed_ = character->getSpeed();
        p.position_ = enemyPositions_.at(j);
        p.index_ = j;
        p.type_ = "enemy";

        addCharacterToQueue(p);
    }

    activePlayer_ = 0;
    heroesDied_ = 0;
    enemiesDied_ = 0;
}

QPixmap BattleManager::getEnemyPicture()
{
    // NOTE: Some hardcoded values, could be added to configuration file
    int picNumber = randInt(1, 4);
    QString path(":\\images\\enemy_" + QString::number(picNumber) + ".png");

    QPixmap pic(path);
    // Return default image if one is missing
    if (!pic.isNull())
        return pic;
    else
        return QPixmap(":\\images\\enemy_1.png");
}

void BattleManager::addCharacterToQueue(chara_ character)
{
    // First element
    if(turnQueue_.size() == 0)
    {
        turnQueue_.push_back(character);
        return;
    }

    // Middle element
    for(unsigned int i = 0; i < turnQueue_.size(); ++i)
    {
        if(turnQueue_[i].speed_ < character.speed_)
        {
            turnQueue_.insert(turnQueue_.begin() + i, character);
            return;
        }
    }

    // Last element
    turnQueue_.push_back(character);
}

// Returns true if enemy character attacked player
bool BattleManager::enemyTurn(bool& targetStatus, bool& gameEnd)
{
    // Call enemy ai function
    bool attack = false;
    std::pair<int,int> target;
    std::pair<int,int> newPosition(0,0);
    targetStatus = enemies_[turnQueue_[activePlayer_].index_]->ai(turnQueue_[activePlayer_].position_, attack, target, heroPositions_, newPosition);

    if(!targetStatus)
    {
        // Mark character dead
        gameEnd = markCharacterDead(target);
    }

    if(attack)
    {
        return true;
    }

    if(newPosition != std::make_pair(0,0))
    {
        changeActivePlayerPosition(newPosition);
    }

    return false;
}

int BattleManager::getAliveHeroes()
{
    return heroPositions_.size() - heroesDied_;
}
