#include <QDebug>

#include "engine.h"

Engine::Engine(GameEngineInterface *game) :
    game_(game)
{
}

Engine::~Engine()
{
    delete game_;
}

void Engine::run()
{
    // Start the game
    game_->start();
}

void Engine::newGame()
{
    game_->reset();
    game_->start();

    emit writeToLog("Started a new game!");
}

void Engine::saveGame()
{
    // Game saving using QDataStream
    emit writeToLog("Game saved (not really)!");
}

void Engine::loadGame()
{
    // Game loading using QDataStream
    // Call start() for the loaded Game object
    emit writeToLog("Game loaded (not really)!");
}

void Engine::countCordinates(int x, int y, QString mouse)
{
    std::pair<int,int> coords = coordinatesToMap(x, y);
    qDebug() << mouse << "Mouse click at position x:" << coords.first << "y:" << coords.second;

    // Send cordinates to game
    game_->cordinatesToWorld(coordinatesToMap(x, y), mouse);
}

void Engine::countCordinates(int x, int y, unsigned int inventorySlot)
{
    std::pair<int,int> coords = coordinatesToMap(x, y);
    qDebug() << "Drop from item slot" << inventorySlot << "at position x:" << coords.first << "y:" << coords.second;

    // Send cordinates to game
    game_->cordinatesToWorld(coordinatesToMap(x, y), inventorySlot);
}

std::pair<int, int> Engine::coordinatesToMap(int x, int y)
{
    // Pictures are streched to fill whole board, so there needs to be compensating numbers
    // NOTE: These values could come from configuration file
    int x_dif = 4;
    int y_dif = 4;

    int index_x = x / (16 + x_dif);
    int index_y = y / (16 + y_dif);
    return std::make_pair(index_x, index_y);
}
