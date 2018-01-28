#include <QDebug>

#include "shopdialog.h"
#include "ui_shopdialog.h"
#include "entityinfoview.h"

ShopDialog::ShopDialog(GameShopInterface *game, QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::ShopDialog),
    game_(game)
{
    ui_->setupUi(this);
    game_ = game;

    QObject::connect(ui_->shopItemList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(updateEntityShop()));
    QObject::connect(ui_->inventoryView, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(updateEntityItem()));


    // The amount of items generated in shop
    shopEquipment_ = 4;
    shopConsumables_ = 3;

    costOfChar_ = 1000;

    // Save location of ui elements for easy access
    character1_ = ui_->character1;
    character2_ = ui_->character2;
    character3_ = ui_->character3;
    character4_ = ui_->character4;

    playerGCValue_ = this->findChild<QLabel*>("playerGCValue");

    inventoryView_ = this->findChild<InventoryView*>("inventoryView");
    shopListView_ = this->findChild<ShopItemList*>("shopItemList");



}

ShopDialog::~ShopDialog()
{
    delete ui_;

    for(unsigned int i; i < shopItems_.size(); ++i)
    {
        delete shopItems_.at(i);
        shopItems_.at(i) = NULL;
    }
}

void ShopDialog::openShop(GameShopInterface* game)
{
    game_ = game;



    updateTabs();
    QDialog::open();
}

void ShopDialog::on_battleButton_clicked()
{
    game_->startBattle();
    close();
}

void ShopDialog::on_buyCharacter_clicked()
{
    // Check if player has money to buy char
    // If true: buy new random char
    if(playerGold_ >= costOfChar_ and game_->addNewCharacter())
    {
        playerGold_ -= costOfChar_;
        playerGCValue_->setNum(static_cast<int>(playerGold_));

        game_->removePlayerGold(costOfChar_);

        updateCharImages();
    }
}

void ShopDialog::on_buyItem_clicked()
{
    unsigned int selected = shopListView_->currentRow();

    if(selected < shopItems_.size() and
       static_cast<int>(playerGold_) >= shopItems_.at(selected)->getPrice()
       and shopItems_.size() != 0)
    {


        // Adds the item to players inventory
        game_->addItem(shopItems_.at(selected));



        // Update to show item in player inventory
        inventoryView_->update();

        // Change gold values for player in game and in shop
        playerGold_ -= shopItems_.at(selected)->getPrice();
        game_->removePlayerGold(shopItems_.at(selected)->getPrice());
        playerGCValue_->setNum(static_cast<int>(playerGold_));

        // Deletes item from shop
        shopListView_->removeItem(selected);

        // Remove bought item from shopDataStorage
        shopItems_.erase(shopItems_.begin() + selected);
    }
}

void ShopDialog::on_character1_clicked()
{
    updateHeroInfo(0);
}

void ShopDialog::on_character2_clicked()
{
    updateHeroInfo(1);
}

void ShopDialog::on_character3_clicked()
{
    updateHeroInfo(2);
}

void ShopDialog::on_character4_clicked()
{
    updateHeroInfo(3);
}

//used To update item info
void ShopDialog::updateEntityShop()
{
    unsigned int selected = shopListView_->currentRow();

    ui_->entityPic->setPixmap(shopItems_.at(selected)->getImage());
    ui_->entityDescription->setText(shopItems_.at(selected)->getDescription());

}
// updates item info
void ShopDialog::updateEntityItem()
{
    inventory_ = game_->getInventoryInfo();
    unsigned int selected = inventoryView_->currentRow();

    ui_->entityPic->setPixmap(inventory_.at(selected)->getImage());
    ui_->entityDescription->setText(inventory_.at(selected)->getDescription());
}

void ShopDialog::on_removeItem_clicked()
{
    unsigned int selected = inventoryView_->currentRow();


    inventory_ = game_->getInventoryInfo();


    //removes elected item
    if(selected <= inventory_.size() )
    {
        game_->removeItem(selected);
        inventory_.erase(inventory_.begin() + selected);
        inventoryView_->update();
    }







}

void ShopDialog::updateTabs()
{
    // Opens shop -> empty old items
    shopListView_->emptyShop();
    shopItems_.clear();


    playerGold_ = game_->getPlayerGold();
    playerGCValue_->setNum(static_cast<int>(playerGold_));

    updateCharImages();

    // Initializes and updates teams inventory to shop view
    inventoryView_->setGame(dynamic_cast<GameInfoInterface*> (game_));
    inventoryView_->update();

    // Generate and add new items to shop
    Item* newItem = NULL;
    for(int i = 0; i < shopEquipment_; ++i)
    {
        // Generates equipment
        newItem = game_->getRandomEquipment();
        addItemToShop(newItem);
    }
    for(int j = 0; j < shopConsumables_; ++j)
    {
        // Generates consumables
        newItem = game_->getRandomConsumable();
        addItemToShop(newItem);
    }

    //get info of chars in tabs
    updateHeroInfo(0);
    return;
}

void ShopDialog::updateCharImages()
{
    std::vector<QPixmap> charImages = game_->getCharImages();

    // Set the images of characters
    // because the image we get from getCharImages is always size of 4.
    // This will not crash with fewer players
    QIcon buttonIcon1(charImages.at(0));
    character1_->setIcon(buttonIcon1);
    QIcon buttonIcon2(charImages.at(1));
    character2_->setIcon(buttonIcon2);
    QIcon buttonIcon3(charImages.at(2));
    character3_->setIcon(buttonIcon3);
    QIcon buttonIcon4(charImages.at(3));
    character4_->setIcon(buttonIcon4);
}

void ShopDialog::addItemToShop(Item *newItem)
{
    shopListView_->addItem(newItem);
    shopItems_.push_back(newItem);
}

void ShopDialog::updateHeroInfo(int heroNum)
{
    game_->changeActivePlayer(heroNum);

    GameInfoInterface::CharStats stats = dynamic_cast<GameInfoInterface*> (game_)->getEntityInfo();

    ui_->pictureValue->setPixmap(stats.charImage);
    // change the values in char info view
    ui_->armorValue->setNum(stats.armor);
    ui_->currentHealthValue->setNum(stats.health);
    ui_->maxHealthValue->setNum(stats.maxHealth);

    ui_->attackValue->setNum(stats.attack);
    ui_->luckValue->setNum(stats.luck);
    ui_->rangeValue->setNum(stats.range);

    ui_->movementValue->setNum(stats.movementPoints);
    ui_->speedValue->setNum(stats.speed);

}
