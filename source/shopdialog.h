#ifndef SHOPDIALOG_H
#define SHOPDIALOG_H

#include <QDialog>
#include <QLabel>

#include "inventoryview.h"
#include "shopitemlist.h"

#include "gameinfointerface.h"
#include "gameshopinterface.h"


namespace Ui {
class ShopDialog;
}

class ShopDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShopDialog(GameShopInterface *game, QWidget *parent = 0);
    ~ShopDialog();

    // Overwritten functionality for open method
    void openShop(GameShopInterface *game);

private slots:

    // Slot for pressing the button and closing the shop window
    void on_battleButton_clicked();

    //player wants to buy new character
    void on_buyCharacter_clicked();

    //played buys selected item
    void on_buyItem_clicked();

    //used when clicked on a char
    void on_character1_clicked();
    void on_character2_clicked();
    void on_character3_clicked();
    void on_character4_clicked();

    void updateEntityShop();
    void updateEntityItem();

    void on_removeItem_clicked();


private:
    // Update tab information
    void updateTabs();
    //updates characters images in shop view
    void updateCharImages();

    void addItemToShop(Item* newItem);

    void updateHeroInfo(int heroNum);



    QPushButton* character1_;
    QPushButton* character2_;
    QPushButton* character3_;
    QPushButton* character4_;

    QLabel* playerGCValue_;

    unsigned int playerGold_;

    Ui::ShopDialog *ui_;
    GameShopInterface *game_;
    // Interface to get information from the Game (model)
    InventoryView *inventoryView_;
    ShopItemList* shopListView_;

    int shopEquipment_;
    int shopConsumables_;

    unsigned int costOfChar_;

    std::vector<Item*> shopItems_;
    std::vector<Item*> inventory_;

};

#endif // SHOPDIALOG_H
