#include "shopitemlist.h"

ShopItemList::ShopItemList(QWidget *parent) :
    QListWidget(parent)
{
}

ShopItemList::~ShopItemList()
{
    QListWidget::clear();
}

// Add given item to shop
void ShopItemList::addItem(Item* item)
{
    // Takes the items name and adds price with it
    // puts the item to shop list
    QIcon itemIcon(QPixmap(item->getImage()));
    QString nameAndPrice = item->getName() + "  (" + QString::number(item->getPrice()) +  " GC) ";
    QListWidgetItem* listItem = new QListWidgetItem(itemIcon, nameAndPrice);
    QListWidget::addItem(listItem);
}

void ShopItemList::removeItem(int row)
{
    QListWidgetItem* item = this->takeItem(row);
    delete item;
}

void ShopItemList::emptyShop()
{
    QListWidget::clear();
}
