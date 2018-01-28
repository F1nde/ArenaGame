#ifndef ITEMGENERATOR_H
#define ITEMGENERATOR_H

#include <vector>
#include <QPixmap>

#include "item.h"

// Class that generates Item objects (Equipment, Consumable)
// with randomly generated attributes (name, icon, stats).
// NOTE: This class is not responsible for releasing the memory
// reserved for the items, it is only a factory for Item objects!
class ItemGenerator
{
public:
    ItemGenerator();
    ~ItemGenerator();

    // Randomly selects consumable configuration and
    // returns a Consumable based on those stats
    Item* generateConsumable();
    // Randomly selects equipment configuration and
    // returns an Equipment based on those stats
    Item* generateEquipment();

private:
    // NOTE: This could be read from a configuration file
    static const int CONF_AMOUNT = 10;

    // Struct to store item configurations read from the
    // item configuration file
    struct ItemConf {
        QString name;
        QString type;
        Item::Buff buff;
    };

    // Reads item configuration from the given file and
    // initialized the given vector with the information
    void readItemConfiguration(QString fileName, std::vector<ItemConf> &itemList);
    // Loads icons for the generated items
    void loadIcons();
    // Conversion from icon type defined in the configuration file
    // to corresponding image
    QPixmap typeToIcon(QString type);

    std::vector<ItemConf> consumables_;
    std::vector<ItemConf> equipment_;

    std::vector<QPixmap> potionIcons_;
    QPixmap foodIcon_;
    QPixmap meleeIcon_;
    QPixmap armorIcon_;
    QPixmap rangedIcon_;
};

#endif // ITEMGENERATOR_H
