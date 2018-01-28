#ifndef AI
#define AI

#include <utility>
#include <vector>

class Ai
{
public:
    virtual ~Ai(){}

    // Functions for actions

    // Find next place to move
    virtual void pathFinding(int movementPoints, std::pair<int,int> activePosition, std::vector<std::pair<int, int> > heroPositions,
                             std::pair<int,int>& newPosition) = 0;

    // Find target to hit
    virtual std::pair<int,int> findTarget(int range, std::pair<int,int> activePosition) = 0;

    // Attack found target
    virtual bool attackTarget(std::pair<int,int> activePosition, std::pair<int,int> target) = 0;
};

#endif // AI
