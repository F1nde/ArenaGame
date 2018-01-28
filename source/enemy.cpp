#include <QDebug>

#include "enemy.h"
#include "hero.h"
#include "helpers.h"
#include "basicai.h"

Enemy::Enemy(QPixmap image, bool block, int stats, World* world):
    Character(image, block)
{
    Q_UNUSED(image);
    Q_UNUSED(block);

    randomizeStats(stats);

    ai_ = new BasicAi(world);
}

Enemy::~Enemy()
{
    delete ai_;
    ai_ = nullptr;
}

bool Enemy::hitTarget(Hero* target, std::pair<int,int>& info)
{
    bool critical = false;

    // Calculate damage
    double rawdamage = sqrt(static_cast<double>(charStats_.attack_));
    int damage = ((charStats_.speed_/2) + (charStats_.attack_ / 2) + static_cast<int>(rawdamage));

    if(charStats_.luck_ > 0)
    {
        int luck = randInt(0,100);

        if(luck < charStats_.luck_)
        {
            damage = damage * 2;
            critical = true;
        }
    }

    info = target->takeDamage(damage);
    return critical;
}

void Enemy::randomizeStats(int pool)
{
    std::vector<int>statBoosts (5,0);
    int stat;

    for(int i = 0; i < 5; ++i)
    {
        stat = randInt(0,pool);
        pool = pool - stat;
        statBoosts[i] = stat;
    }

    charStats_.health_ = 10 + statBoosts[0];
    charStats_.maxHealth_ = charStats_.health_;
    charStats_.attack_ = 2 + statBoosts[1];
    charStats_.speed_ = 0 + statBoosts[2];
    charStats_.armor_ = 0 + (statBoosts[3] / 2);
    charStats_.luck_ = 0 + (statBoosts[4] / 2);

    charStats_.movementPoints_ = 4 + randInt(0,1);
    charStats_.range_ = 1 + randInt(0,5);
    charStats_.name_ = "ENEMY";
}

bool Enemy::ai(std::pair<int,int> activePosition, bool& attack, std::pair<int,int>& target,
               std::vector<std::pair<int, int> > heroPositions, std::pair<int,int>& newPosition)
{
    // Attack target if there is one inside attack range
    target = ai_->findTarget(charStats_.range_, activePosition);

    if(target != std::make_pair(0,0))
    {
        bool targetStatus = ai_->attackTarget(activePosition, target);
        attack = true;

        if(!targetStatus)
        {
            return false;
        }
    }
    else
    {
        // No target in range
        ai_->pathFinding(charStats_.movementPoints_, activePosition, heroPositions, newPosition);
    }

    return true;
}
