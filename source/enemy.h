#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"
#include "ai.h"
#include "world.h"

class Hero;

class Enemy: public Character
{
public:
    Enemy(QPixmap image, bool block, int stats, World* world);
    ~Enemy();

    // Method to hit given target Hero, also returns
    // pair for logging
    bool hitTarget(Hero* target, std::pair<int,int>& info);
    // Randomizes and sets Enemy's stats
    void randomizeStats(int pool);
    // Function that controls enemy actions
    bool ai(std::pair<int,int> activePosition, bool& attack, std::pair<int,int>& target,
            std::vector<std::pair<int, int> > heroPositions, std::pair<int,int>& newPosition);

private:
    Ai* ai_;
};

#endif // ENEMY_H
