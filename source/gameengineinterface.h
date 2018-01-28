#ifndef GAMEENGINEINTERFACE_H
#define GAMEENGINEINTERFACE_H

// Interface between Game (model) and Engine (controller)

class GameEngineInterface
{
public:
    virtual ~GameEngineInterface(){}

    virtual void start() = 0;
    virtual void reset() = 0;
    virtual void cordinatesToWorld(std::pair<int,int> target, QString mouse) = 0;
    virtual void cordinatesToWorld(std::pair<int,int> target, unsigned int slot) = 0;
};

#endif // GAMEENGINEINTERFACE_H
