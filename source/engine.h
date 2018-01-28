#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QString>

#include "gameengineinterface.h"

class Engine : public QObject
{
    Q_OBJECT

public:
    Engine(GameEngineInterface *game);
    ~Engine();

    // Run the engine
    void run();

    // Create new game or save/load game
    void newGame();
    void saveGame();
    void loadGame();

    // Method for translating view clicks into the game world
    void countCordinates(int x, int y, QString mouse);
    // Overloaded method for dropping inventory items into the view
    void countCordinates(int x, int y, unsigned int inventorySlot);

signals:
    // For logging directly from Engine
    void writeToLog(QString msg);

private:
    std::pair<int, int> coordinatesToMap(int x, int y);
    GameEngineInterface *game_;
};

#endif // ENGINE_H
