#ifndef SHOPITEMLIST_H
#define SHOPITEMLIST_H

#include <QListWidget>

#include "item.h"

class ShopItemList : public QListWidget
{
    Q_OBJECT

public:
    explicit ShopItemList(QWidget *parent = 0);
    ~ShopItemList();

    // Adds a item to shop
    void addItem(Item* item);

    void removeItem(int row);

    // Empties shop
    void emptyShop();

public slots:
};

#endif // SHOPITEMLIST_H
