#ifndef INVENTORYVIEW_H
#define INVENTORYVIEW_H

#include <QListWidget>

#include "gameinfointerface.h"

class InventoryView : public QListWidget
{
    Q_OBJECT

public:
    explicit InventoryView(QWidget *parent);
    ~InventoryView();

    // Sets the reference to Game's info interface
    void setGame(GameInfoInterface *game);
    // Updates the inventory view
    // - fetches current info through the interface
    // - updates the info to the view
    void update();

    void itemClicked(QListWidgetItem *item);

    void removeItem();

    void deleteAll();

private:
    GameInfoInterface *game_;
};

#endif // INVENTORYVIEW_H
