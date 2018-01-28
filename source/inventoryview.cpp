#include <vector>

#include "inventoryview.h"
#include "item.h"

InventoryView::InventoryView(QWidget *parent) :
    QListWidget(parent)
{
}

InventoryView::~InventoryView()
{
    QListWidget::clear();
}

void InventoryView::setGame(GameInfoInterface *game)
{
    game_ = game;
}

void InventoryView::update()
{
    QListWidget::clear();

    // Get the current items through the interface
    std::vector<Item*> items = game_->getInventoryInfo();

    if(items.size() == 0)
    {
        return;
    }

    // Loop through items, get their info and create
    // bunch of QListWidgetItems to be showed on the screen
    for(unsigned int i = 0; i < items.size(); ++i)
    {
        QIcon itemIcon(QPixmap(items.at(i)->getImage()));
        QListWidgetItem* listItem = new QListWidgetItem(itemIcon, items.at(i)->getName());
        QListWidget::addItem(listItem);
    }
}



void InventoryView::deleteAll()
{
    QListWidget::clear();
}
