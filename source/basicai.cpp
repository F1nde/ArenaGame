#include <QDebug>

#include "basicai.h"

BasicAi::BasicAi(World* world)
{
    world_ = world;
}

BasicAi::~BasicAi(){}

void BasicAi::pathFinding(int movementPoints, std::pair<int,int> activePosition,
                          std::vector<std::pair<int, int> > heroPositions, std::pair<int,int>& newPosition)
{
    std::vector<std::pair<int, int> > possiblePosition;

    int x_dif;
    int y_dif;
    int x_newPosition;
    int y_newPosition;

    // Direction defines where we start to find possible x cordinate
    // from right to left (target is lower than the enemy) (true)
    // or from left to right (target is higher than the enemy) (false)
    bool direction;

    // Check hero positions and make possible positions

    // First loop is to make sure that enemy will move somewhere.
    // If all optimal positions are blocked -> start changing the target position (= y_dif)
    for(int k = 0; k < 100; ++k)
    {
        // Go through hero positions and store possible move positions
        for(unsigned int i = 0; i < heroPositions.size(); ++i)
        {
            //qDebug() << "My position:" << activePosition.first << activePosition.second;
            //qDebug() << "Enemy position:" << heroPositions[i].first << heroPositions[i].second;
            x_dif = activePosition.first - heroPositions[i].first - (k/3);
            y_dif = activePosition.second - heroPositions[i].second - k;
            //qDebug() << "position differences:" << x_dif << y_dif;

            if(y_dif > 0)
            {
                if(y_dif > movementPoints)
                {
                    y_newPosition = activePosition.second - movementPoints;
                }
                else
                {
                    y_newPosition = activePosition.second - y_dif;
                }
            }
            else if(y_dif < 0)
            {
                if(y_dif < (movementPoints * -1))
                {
                    y_newPosition = activePosition.second + movementPoints;
                }
                else
                {
                    y_newPosition = activePosition.second + abs(y_dif);
                }
            }
            else
            {
                y_newPosition = activePosition.second;
            }

            if(x_dif > 0)
            {
                direction = true;

                if(x_dif > movementPoints)
                {
                    x_newPosition = activePosition.first - movementPoints;
                }
                else
                {
                    x_newPosition = activePosition.first - x_dif;
                }
            }
            else if(x_dif < 0)
            {
                direction = false;

                if(x_dif < (movementPoints * -1))
                {
                    x_newPosition = activePosition.first + movementPoints;
                }
                else
                {
                    x_newPosition = activePosition.first + abs(x_dif);
                }
            }
            else
            {
                x_newPosition = activePosition.first;
            }

            // Store possible position
            possiblePosition.push_back(std::make_pair(x_newPosition,y_newPosition));
        }

        // After possible positions are stored, start investigating them.

        bool placeFound;
        bool move;
        std::pair<int,int> p;

        for(unsigned int i = 0; i < possiblePosition.size(); ++i)
        {
            if(direction)
            {
                // Check possible x cordinates
                for(int j = 0; j < movementPoints; ++j)
                {
                    p = std::make_pair(possiblePosition[i].first + j, possiblePosition[i].second);
                    Entity* entity = world_->getObject(p);

                    // If no entity -> try to move there
                    if(entity == nullptr)
                    {
                        // Movement can still fail
                        // For example not enought movement points
                        move = world_->moveUnit(p,activePosition);

                        if(move)
                        {
                            newPosition = p;
                            placeFound = true;
                            break;
                        }
                    }
                }
            }
            else
            {
                // Check possible x cordinates
                for(int j = 0; j > (movementPoints * -1); --j)
                {
                    p = std::make_pair(possiblePosition[i].first + j, possiblePosition[i].second);
                    Entity* entity = world_->getObject(p);

                    // If no entity -> try to move there
                    if(entity == nullptr)
                    {
                        // Movement can still fail
                        // For example not enought movement points
                        move = world_->moveUnit(p,activePosition);

                        if(move)
                        {
                            newPosition = p;
                            placeFound = true;
                            break;
                        }
                    }
                }
            }

            // Found new position?
            if(placeFound)
            {
                break;
            }
        }

        // Found new position?
        if(placeFound)
        {
            break;
        }
    }
}

std::pair<int,int> BasicAi::findTarget(int range, std::pair<int,int> position)
{
    std::vector<std::pair<int,int> > heroes = world_->findHeroes(position, range);

    if(heroes.size() == 0)
    {
        qDebug() << "Enemy: No nearby targets.";
        return std::make_pair(0,0);
    }
    else
    {
        // Select target from vector
        qDebug() << "Enemy: Found Hero!";

        // NOTE: This could be also made random
        return heroes[0];
    }
}

bool BasicAi::attackTarget(std::pair<int,int> activePosition, std::pair<int,int> target)
{
    qDebug() << "Enemy: Attacking hero!";
    bool status = world_->attackTarget(target,activePosition,"enemy");
    return status;
}
