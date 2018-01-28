#ifndef GAME_H
#define GAME_H

#include "gamewindowinterface.h"
#include "gameengineinterface.h"
#include "gameinfointerface.h"
#include "gameshopinterface.h"

#include "tile.h"
#include "hero.h"
#include "world.h"
#include "battlemanager.h"
#include "inventory.h"
#include "itemgenerator.h"
#include "character.h"

#include <QString>
#include <vector>
#include <memory>

class Game : public QObject, public GameWindowInterface, public GameEngineInterface, public GameInfoInterface, public GameShopInterface
{
    Q_OBJECT

public:
    Game();
    ~Game();

    // GameEngineInterface
    // ===================
    // Starts the actual game. Initialization is done in the constructor.
    void start();
    // Resets the game, making it ready for a new game
    void reset();
    // Represents click to the Game world. Overloaded for item drops into tile.
    void cordinatesToWorld(std::pair<int,int> target, QString mouse);
    void cordinatesToWorld(std::pair<int,int> target, unsigned int slot);

    // GameInfoInterface
    // =================
    // Return info about the Inventory
    std::vector<Item*> getInventoryInfo();
    // Returns info about the selected Entity
    CharStats getEntityInfo();
    // Returns general game information (level, score etc.)
    GameInfo getGameInfo();

    std::vector <QPixmap>getCharImages();

    //makes items
    Item *getRandomEquipment();
    Item *getRandomConsumable();

    void addItem(Item* item);

    //gives the amount of gold the player has
    unsigned int getPlayerGold();
    void removePlayerGold(unsigned int gold);

    //sends back if made new char
    //also adds a new character if not over limit
    bool addNewCharacter();

    // GameShopInterface
    // =================
    // Starts the next battle
    void startBattle();

    void removeItem(unsigned int item);

signals:
    // Signals for communicating with the view (MainWindow)
    void writeToLog(QString msg);
    void updateView(std::vector<World::change> changes);
    void changeMapVisibility(bool visible);
    void updateInventory();
    void updateEntityInfoView();
    void updateGameInfo();
    void gameEnded(unsigned int battlesWon);
    void openShop();

private:
    // Ends current map
    void endBattle(QString status);
    // Load and instantiate new World object from the given file
    void loadWorldFromFile(QString fileName);
    // Creates and initializes BattleManager object
    void createBattleManager(int difficulty);

    // Adds Heroes to the current map spawnpoints
    void addHeroesToMap();
    // Creates Heroes
    void initializeHeroes();
    // Changes turn from player's to enemy's
    void changeTurn();

    // Randomizes new arena for the next battle
    void selectNewArena();

    // Generates random hero picture
    QPixmap getHeroPicture();

    // change the active hero if able
    // used in shop hero info
    void changeActivePlayer(int heroNum);

    // Data structure holding the state of the world/map
    World *world_;
    std::vector<Hero*> heroes_;
    Entity* activeEntity_;
    BattleManager* battleManager_;
    Inventory* inventory_;
    ItemGenerator* itemGenerator_;

    int battleDifficulty_;
    QStringList maps_;
};

#endif // GAME_H
