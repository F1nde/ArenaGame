#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "itemgenerator.h"
#include "consumable.h"
#include "equipment.h"
#include "helpers.h"

using std::vector;

ItemGenerator::ItemGenerator()
{
    // Read item configurations from a file
    // Consumables
    readItemConfiguration(":\\items\\consumables.txt", consumables_);
    // Equipment
    readItemConfiguration(":\\items\\equipment.txt", equipment_);

    // Load icons
    loadIcons();
}

ItemGenerator::~ItemGenerator()
{
}

Item* ItemGenerator::generateConsumable()
{
    // Randomly generate item id
    int id = randInt(0, consumables_.size()-1);
    Item* newItem = new Consumable(typeToIcon(consumables_.at(id).type), true,
                                   consumables_.at(id).name, consumables_.at(id).buff);
    return newItem;
}

Item* ItemGenerator::generateEquipment()
{
    // Randomly generate item id
    int id = randInt(0, equipment_.size()-1);
    Item* newItem = new Equipment(typeToIcon(equipment_.at(id).type), true,
                                  equipment_.at(id).name, equipment_.at(id).buff);
    return newItem;
}

void ItemGenerator::readItemConfiguration(QString fileName, vector<ItemConf> &itemList)
{
    QFile file(fileName);
    QString line;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream data(&file);

        // Read lines until they end
        line = data.readLine();
        while(!line.isNull()) {
            // NOTE: Here we expect data to be valid!
            QStringList conf = line.split(",");
            // At least check the size of the configuration struct
            if (conf.size() == CONF_AMOUNT) {
                // Get name, type and buff data from the line
                QString name = conf.at(0);
                QString type = conf.at(1);
                Item::Buff buff = {conf.at(2).toInt(), conf.at(3).toInt(), conf.at(4).toInt(), conf.at(5).toInt(),
                                   conf.at(6).toInt(), conf.at(7).toInt(), conf.at(8).toInt(), conf.at(9).toInt()};
                ItemConf newConf = {name, type, buff};

                itemList.push_back(newConf);
            }
            else {
                qDebug() << "Invalid data in the configuration file!";
            }
            line = data.readLine();
        }
    }
}

void ItemGenerator::loadIcons()
{
    // Potion icons
    potionIcons_.push_back(QPixmap((":\\images\\consumable_potion_1.png")));
    potionIcons_.push_back(QPixmap((":\\images\\consumable_potion_2.png")));
    potionIcons_.push_back(QPixmap((":\\images\\consumable_potion_3.png")));

    // Meal icons
    foodIcon_ = QPixmap((":\\images\\consumable_meal.png"));

    // Equipment icons
    meleeIcon_ = QPixmap((":\\images\\equip_melee.png"));
    armorIcon_ = QPixmap((":\\images\\equip_armor.png"));
    rangedIcon_ = QPixmap((":\\images\\equip_ranged.png"));
}

QPixmap ItemGenerator::typeToIcon(QString type)
{
    QPixmap icon;
    if (type == "potion") {
        int i = randInt(0, potionIcons_.size()-1);
        icon = potionIcons_.at(i);
    }
    else if (type == "food") {
        icon = foodIcon_;
    }
    else if (type == "melee") {
        icon = meleeIcon_;
    }
    else if (type == "armor") {
        icon = armorIcon_;
    }
    else if (type == "ranged") {
        icon = rangedIcon_;
    }
    else {
        // Type not found, use default image
        icon = meleeIcon_;
    }
    return icon;
}
