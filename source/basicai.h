#ifndef BASICAI
#define BASICAI

#include "ai.h"
#include "world.h"

class BasicAi : public Ai
{
public:
    BasicAi(World* world);
    ~BasicAi();

    // Functions for actions

    // Find next place to move
    void pathFinding(int movementPoints, std::pair<int,int> activePosition, std::vector<std::pair<int, int> > heroPositions,
                     std::pair<int,int>& newPosition);

    // Find target to hit
    std::pair<int,int> findTarget(int range, std::pair<int,int> activePosition);

    // Attack target
    bool attackTarget(std::pair<int,int> activePosition, std::pair<int,int> target);

private:
    World* world_;
};

#endif // BASICAI
