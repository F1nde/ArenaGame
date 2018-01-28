#ifndef BATTLEMANAGER
#define BATTLEMANAGER

#include "entity.h"
#include "world.h"
#include "enemy.h"

class BattleManager
{
public:
    BattleManager(World* world, int tier);
    ~BattleManager();

    // Returns active players turn
    std::pair<int,int> playersTurn();
    bool changeTurn();
    void changeActivePlayerPosition(std::pair<int,int> position);
    void addCharacter(std::pair<int,int> position,QString type);
    bool markCharacterDead(std::pair<int, int> position);
    bool enemyTurn(bool& targetStatus, bool& gameEnd);
    // Returns information about alive heroes
    int getAliveHeroes();

private:
    void createEnemies(int tier);
    void removeCharacter(std::pair<int,int> position, QString type);
    void resetTurnQueue();
    QPixmap getEnemyPicture();

    // Data to turnOrder
    // int = index in character vector
    // QSting = Enemy or Hero
    struct chara_
    {
        int speed_;
        int index_;
        QString type_;
        std::pair<int, int> position_;
        bool status_;
    };

    void addCharacterToQueue(chara_ character);

    std::vector<chara_> turnQueue_;
    unsigned int activePlayer_;

    // Enemies
    std::vector<Enemy*> enemies_;

    // Character positions
    std::vector<std::pair<int, int> > heroPositions_;
    std::vector<std::pair<int, int> > enemyPositions_;

    unsigned int heroesDied_;
    unsigned int enemiesDied_;

    World* world_;
};

#endif // BATTLEMANAGER
