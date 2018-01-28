#include <QDir>
#include <QDebug>
#include <QFile>
#include <QCoreApplication>
#include <QtGlobal>
#include <memory>
#include <unistd.h>

#include "game.h"
#include "helpers.h"
#include "world.h"
#include "inventory.h"
#include "equipment.h"
#include "consumable.h"

Game::Game() :
    world_(nullptr), battleManager_(nullptr), inventory_(nullptr), battleDifficulty_(1)
{
}

Game::~Game()
{
    delete world_;
    delete battleManager_;
    delete inventory_;
    delete itemGenerator_;

    for(unsigned int i = 0; i < heroes_.size(); ++i)
    {
        delete heroes_.at(i);
        heroes_.at(i) = NULL;
    }
}

void Game::start()
{
    qDebug() << "Starting the game";

    // Initialize arena vector
    QStringList filter("*.txt");
    QDir directory(":\\arena\\");
    maps_ = directory.entryList(filter);

    for(int i = 0; i < maps_.size(); ++i)
    {
        qDebug() << "Found map:" << maps_[i];
    }

    qDebug() << "ItemGenerator initialized";
    itemGenerator_ = new ItemGenerator();

    selectNewArena();

    qDebug() << "Inventory initialized";
    inventory_ = new Inventory(10, itemGenerator_);

    initializeHeroes();

    emit updateView(world_->getChanges());
    emit updateInventory();

    // Hide map
    emit changeMapVisibility(false);
    //start game -> open shop
    emit openShop();
}

void Game::reset()
{
    delete world_;
    world_ = nullptr;
    delete battleManager_;
    battleManager_ = nullptr;
    delete inventory_;
    inventory_ = nullptr;
    delete itemGenerator_;
    itemGenerator_ = nullptr;

    activeEntity_ = nullptr;
    battleDifficulty_ = 1;

    for(unsigned int i = 0; i < heroes_.size(); ++i) {
        delete heroes_.at(i);
    }
    heroes_.clear();
}

void Game::startBattle()
{
    // Create Battlemanager
    createBattleManager(battleDifficulty_);

    // Set heroes to map
    addHeroesToMap();
    emit updateView(world_->getChanges());

    // Make sure that player gets first turn
    while(!(battleManager_->changeTurn())){}

    // Set frame to active player position
    world_->addAllowedArea(battleManager_->playersTurn());
    world_->changeSelectedPlayer(battleManager_->playersTurn());

    emit updateView(world_->getChanges());
    emit updateGameInfo();

    // gets the first active player and...
    activeEntity_ = world_->getObject(battleManager_->playersTurn());
    // ...puts its info on the gameWindow
    emit updateEntityInfoView();

    // Show map
    emit changeMapVisibility(true);

    //update bought items to shop
    emit updateInventory();
}

void Game::removeItem(unsigned int item)
{
    inventory_->removeItem(item);
}

void Game::endBattle(QString status)
{
    // Generate rewards and make next battle harder
    if(status == "win")
    {
        // Give player a random amount of gold
        int rewardGold = randInt(1, 99) * battleDifficulty_;
        inventory_->addGold(rewardGold);
        ++battleDifficulty_;
    }

    // Initialize heroes to have 1 health
    for(unsigned int i = 0; i < heroes_.size(); ++i)
    {
        if(heroes_[i]->getHealth() == 0)
        {
            heroes_[i]->reviveHero();
        }
    }

    // Load next map
    selectNewArena();
    emit updateView(world_->getChanges());

    // Hide map
    emit changeMapVisibility(false);
    // game ended ->show shop
    emit openShop();
}

void Game::cordinatesToWorld(std::pair<int,int> target, QString mouse)
{
    // Get pointer to possible Entity in clicked Tile
    Entity* entity = world_->getObject(target);

    if(mouse == "left")
    {
        if(entity == NULL)
        {
            qDebug() << "No entity on targetet Tile";
        }
        else
        {
            Character* hero = dynamic_cast<Character*>(entity);

            // Checks if the target is a character
            // If not, does nothing.
            if (hero != nullptr)
            {
                //updates the infoView about the clicked character
                activeEntity_ = entity;
                emit updateEntityInfoView();
            }

            qDebug() << "Entity x on targeted Tile";
        }
    }
    else if(mouse == "right")
    {
        // Get position of active player
        std::pair<int,int> activePosition = battleManager_->playersTurn();

        if(entity == NULL)
        {
            // Call moveUnit. If move changed worldstate -> player turn needs to be changed
            bool change = world_->moveUnit(target,activePosition);

            if(change)
            {
                // Change active players position
                battleManager_->changeActivePlayerPosition(target);

                emit updateView(world_->getChanges());
                QCoreApplication::processEvents();
                changeTurn();
            }
            else
            {
                emit writeToLog("Can't move to target position: not enought movement points.");
            }
        }
        else
        {
            // Identify entity and act properly

            Enemy* enemy = dynamic_cast<Enemy*>(entity);

            // Enemy
            if(enemy != nullptr)
            {
                // Check if enemy is in range of player
                bool range = world_->rangeCheck(target, activePosition, "attack");

                if(range)
                {
                    bool targetStatus = world_->attackTarget(target, activePosition, "hero");

                    // Get information from last attack and write to log
                    std::pair<int,int> info;
                    bool critical = world_->getAttackInfo(info);
                    QString damage = QString::number(info.first);
                    QString remainingHealth = QString::number(info.second);
                    if(critical)
                    {
                        emit writeToLog("You <b>CRITICAL HIT</b> opponent for <b>"+ damage +" damage</b>. Opponent has <b>" + remainingHealth + " health left.</b>");
                    }
                    else
                    {
                        emit writeToLog("You hit opponent for <b>"+ damage +" damage</b>. Opponent got <b>"+ remainingHealth +" health left</b>.");
                    }

                    if(!targetStatus)
                    {
                        // Mark character dead
                        bool gameEnd = battleManager_->markCharacterDead(target);
                        emit writeToLog("Enemy died!");

                        if(gameEnd)
                        {
                            emit writeToLog("All enemies are dead. <b>You won the battle!</b>");
                            endBattle("win");
                            emit updateGameInfo();
                            return;
                        }
                    }

                    changeTurn();
                }
                else
                {
                    qDebug() << "Can't attack target opponent: not enought attack range.";
                }

                return;
            }

            Item* item = dynamic_cast<Item*>(entity);

            // Item
            if(item != nullptr)
            {
                qDebug() << "Item in that position.";

                // Check if item is in range of player
                bool range = world_->rangeCheck(target,activePosition, "movement");

                if(range)
                {
                    Inventory::InventoryStatus status = inventory_->addItem(item);
                    world_->removeItem(target);

                    if (status == Inventory::InventoryStatus::OK) {
                        emit writeToLog("Collected <b>"+ item->getName() + "!");
                        emit updateInventory();
                    }
                    else {
                        emit writeToLog("Your inventory is full. You tried to stuff the <b>"+ item->getName()
                                        + "</b> into your full bag and it broke!");
                        delete item;
                    }
                    QCoreApplication::processEvents();

                    world_->moveUnit(target,activePosition);
                    battleManager_->changeActivePlayerPosition(target);

                    emit updateView(world_->getChanges());
                    QCoreApplication::processEvents();
                    changeTurn();
                }
                else
                {
                    qDebug() << "Can't move to target position: not enought movement points.";
                }

                return;
            }

            Hero* hero = dynamic_cast<Hero*>(entity);

            // Hero in position
            if(hero != nullptr)
            {
                qDebug() << "Hero in that position.";
                emit writeToLog("Hero in that position.");
                return;
            }

            // Else: Targeted Entity is misc or ground. No changes required.
        }
    }
    else
    {
        qDebug() << "Error: no mouse defined (left or right)";
    }
}

void Game::cordinatesToWorld(std::pair<int, int> target, unsigned int slot)
{
    // Get pointer to possible Entity in clicked Tile
    Entity* entity = world_->getObject(target);

    if(entity != nullptr) {
        Hero* hero = dynamic_cast<Hero*>(entity);

        // Hero in position
        if(hero != nullptr) {
            Item *item = inventory_->getItemInSlot(slot);

            // Determine is the item Equipment or Consumable
            Equipment *equipment = dynamic_cast<Equipment*>(item);
            if (equipment != nullptr) {
                Item *temp = hero->changeEquipment(equipment);
                // If Hero already had some equipment, put it back to inventory
                if (temp != nullptr)
                    inventory_->addItem(temp);
                emit writeToLog("Equipped <b>"+ item->getName() +"</b> (<i>"+item->getDescription()
                                +"</i>  ) on <b><font color=\"green\">" + hero->getName() + "</font></b>!");
            }
            else {
                emit writeToLog("Consumed <b>"+ item->getName() +"</b> (<i>"+item->getDescription()
                                +"</i>  ) on <b><font color=\"green\">" + hero->getName() + "</font></b>!");
                hero->consumeItem(dynamic_cast<Consumable*>(item));
            }

            emit updateInventory();
            emit updateEntityInfoView();
        }
        else {
            // Tile has an Entity but it is not a Hero
            emit writeToLog("<b>Items can only be used on allied characters!</b>");
        }
    }
    else {
        // No hero or something else on the tile
        // NOTE: Items can be only used on allied characters!
        emit writeToLog("<b>Items can only be used on allied characters!</b>");
    }
}

void Game::loadWorldFromFile(QString fileName)
{
    // Delete the possible old world
    if (world_ != nullptr) delete world_;
    // Create a new world
    world_ = new World(fileName, itemGenerator_);
}

void Game::createBattleManager(int difficulty)
{
    if (battleManager_ != nullptr) delete battleManager_;
    battleManager_ = new BattleManager(world_, difficulty);
}

void Game::addHeroesToMap()
{
    // Add heroes to game map
    for(unsigned int i = 0; i < heroes_.size(); ++i)
    {
        // Add player to game map
        std::pair<int,int> cordinates = world_->addCharacter(heroes_.at(i),"player");

        // Send players cordinates to battleManager
        battleManager_->addCharacter(cordinates, "player");
    }
}

void Game::initializeHeroes()
{
    // Create starting heroes
    Hero* hero_1 = new Hero(getHeroPicture(), true, 20);
    heroes_.push_back(hero_1);

    Hero* hero_2 = new Hero(getHeroPicture(), true, 20);
    heroes_.push_back(hero_2);
}

std::vector<Item *> Game::getInventoryInfo()
{
    return inventory_->getCurrentItems();
}

// takes all the needed information from charracter and sends them back as
// struct named CharStats
GameInfoInterface::CharStats Game::getEntityInfo()
{
    CharStats charStats;

    Character* activeCharacter = static_cast<Character*>(activeEntity_);

    charStats.charImage = activeCharacter->getImage();

    charStats.health = activeCharacter->getHealth();
    charStats.maxHealth = activeCharacter->getMaxHealth();
    charStats.armor = activeCharacter->getArmor();

    charStats.attack = activeCharacter->getAttack();
    charStats.luck = activeCharacter->getLuck();
    charStats.range = activeCharacter->getAttackRange();

    charStats.movementPoints = activeCharacter->getMovementPoints();
    charStats.speed = activeCharacter->getSpeed();

    return charStats;
}

GameInfoInterface::GameInfo Game::getGameInfo()
{
    GameInfo info = {world_->getName(),(int) (battleDifficulty_ - 1), (int) inventory_->getCurrentGold(),
                     (int) battleManager_->getAliveHeroes()};
    return info;
}

std::vector<QPixmap> Game::getCharImages()
{
    // Initialize four pointers (there are for playable characters) to NULL
    std::vector <QPixmap> charImages(4, QPixmap());

    for(unsigned int i = 0; i < heroes_.size(); ++i)
    {
        charImages.at(i) = heroes_.at(i)->getImage();
    }
    return charImages;
}

Item *Game::getRandomEquipment()
{
    return itemGenerator_->generateEquipment();
}

Item *Game::getRandomConsumable()
{
    return itemGenerator_->generateConsumable();
}

void Game::addItem(Item* item)
{
    inventory_->addItem(item);
}

unsigned int Game::getPlayerGold()
{
    return inventory_->getCurrentGold();
}

void Game::removePlayerGold(unsigned int gold)
{
    inventory_->removeGold(gold);
}

//adds new character, but only if there are under 4 heroes on team
// true = made new char, fales = not room
bool Game::addNewCharacter()
{
    // there is room for a new char
    // makes new char
    if (heroes_.size() < 4)
    {
        Hero* hero = new Hero(getHeroPicture(), true, 20);
        heroes_.push_back(hero);
        return true;
    }
    else
    {
        return false;
    }
}

void Game::changeTurn()
{
    emit updateView(world_->getChanges());
    QCoreApplication::processEvents();

    bool nextPlayerHero = battleManager_->changeTurn();

    // Remove old movement area.
    world_->removeAllowedArea();

    // Update map without old movement/attack area
    emit updateView(world_->getChanges());
    QCoreApplication::processEvents();

    std::pair<int,int> activePosition;
    // Loop while there is enemy turns before player's turn
    while(!(nextPlayerHero))
    {
        world_->changeSelectedPlayer(battleManager_->playersTurn());

        activeEntity_ = world_->getObject(battleManager_->playersTurn());
        emit updateEntityInfoView();

        emit updateView(world_->getChanges());
        QCoreApplication::processEvents();

        activePosition = battleManager_->playersTurn();

        // Enemy turns here
        qDebug() << "Enemy turn now!";

        sleep(1);

        bool targetStatus = true;
        bool gameEnd = false;
        bool attack = battleManager_->enemyTurn(targetStatus, gameEnd);

        if(attack)
        {
            std::pair<int,int> info;
            bool critical = world_->getAttackInfo(info);
            QString damage = QString::number(info.first);
            QString remainingHealth = QString::number(info.second);

            if(critical)
            {
                emit writeToLog("Enemy <b>CRITICAL HIT</b> player for <b>"+ damage +" damage</b>. Player has <b>" + remainingHealth + " health left.</b>");
            }
            else
            {
                emit writeToLog("Enemy hit player for <b>"+ damage +" damage</b>. Player got <b>"+ remainingHealth +" health left</b>.");
            }

            if(!targetStatus)
            {
                emit writeToLog("Hero died!");

                if(gameEnd)
                {
                    emit writeToLog("All heroes are dead. You lost the battle!");
                    emit gameEnded(battleDifficulty_ - 1);
                    return;
                }

                emit updateGameInfo();
            }
        }

        emit updateView(world_->getChanges());
        QCoreApplication::processEvents();

        nextPlayerHero = battleManager_->changeTurn();
    }

    activeEntity_ = world_->getObject(battleManager_->playersTurn());
    emit updateEntityInfoView();

    emit updateView(world_->getChanges());
    QCoreApplication::processEvents();

    world_->addAllowedArea(battleManager_->playersTurn());

    emit updateView(world_->getChanges());
    QCoreApplication::processEvents();

    world_->changeSelectedPlayer(battleManager_->playersTurn());

    emit updateView(world_->getChanges());
    QCoreApplication::processEvents();
}

void Game::selectNewArena()
{
    int randomNumber = randInt(0, maps_.size()-1);
    QString path = ":\\arena\\";

    // Select next map
    loadWorldFromFile(path + maps_[randomNumber]);
}

QPixmap Game::getHeroPicture()
{
    // NOTE: Some hardcoded values, could be added to configuration file
    int picNumber = randInt(1, 4);
    QString path(":\\images\\character_" + QString::number(picNumber) + ".png");

    QPixmap pic(path);
    // Return default image if one is missing
    if (!pic.isNull())
        return pic;
    else
        return QPixmap(":\\images\\character_1.png");
}

// change the active hero if able
// used in shop hero info
void Game::changeActivePlayer(int heroNum)
{
    if((static_cast<unsigned int>(heroNum)+1) <= heroes_.size())
    {
        activeEntity_ = heroes_.at(heroNum);

    }
}
