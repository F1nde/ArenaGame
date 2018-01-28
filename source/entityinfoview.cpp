#include <QString>
#include <QDebug>

#include "entityinfoview.h"

EntityInfoView::EntityInfoView(QWidget *parent) :
    QFrame(parent)

{
}

EntityInfoView::~EntityInfoView()
{
}

void EntityInfoView::setGame(GameInfoInterface *game)
{
    game_ = game;

    //make pointers for all the labels involved in char stats for later use
    pic_ = this->findChild<QLabel*>("pictureValue");

    armor_ = this->findChild<QLabel*>("armorValue");
    currentHealth_ = this->findChild<QLabel*>("currentHealthValue");
    maxHealth_ = this->findChild<QLabel*>("maxHealthValue");

    attack_ = this->findChild<QLabel*>("attackValue");
    luck_ = this->findChild<QLabel*>("luckValue");
    range_ = this->findChild<QLabel*>("rangeValue");

    movement_ = this->findChild<QLabel*>("movementValue");
    speed_ = this->findChild<QLabel*>("speedValue");
}

void EntityInfoView::update()
{
    // take stats from active char
    GameInfoInterface::CharStats stats = game_->getEntityInfo();

    pic_->setPixmap(stats.charImage);
    // change the values in char info view
    armor_->setNum(stats.armor);
    currentHealth_->setNum(stats.health);
    maxHealth_->setNum(stats.maxHealth);

    attack_->setNum(stats.attack);
    luck_->setNum(stats.luck);
    range_->setNum(stats.range);

    movement_->setNum(stats.movementPoints);
    speed_->setNum(stats.speed);

    Q_UNUSED(stats);
}
