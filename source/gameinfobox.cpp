#include <QString>

#include "gameinfobox.h"

GameInfoBox::GameInfoBox(QWidget *parent) :
    QTabWidget(parent),
    game_(nullptr),
    generalLabels_({nullptr, nullptr, nullptr, nullptr}),
    statLabels_({nullptr})
{
}

GameInfoBox::~GameInfoBox()
{
}

void GameInfoBox::setGame(GameInfoInterface *game)
{
    game_ = game;
    // Get pointers to UI labels
    getLabelPointers();
}

void GameInfoBox::update()
{
    GameInfoInterface::GameInfo gameInfo = game_->getGameInfo();

    // Set general label texts according to game information
    generalLabels_.arenaName->setText(gameInfo.arenaName);
    generalLabels_.battlesWon->setText(QString::number(gameInfo.battlesWon));
    generalLabels_.gold->setText(QString::number(gameInfo.gold));
    generalLabels_.heroesAlive->setText(QString::number(gameInfo.heroesAlive));
}

void GameInfoBox::getLabelPointers()
{
    generalLabels_.arenaName = this->findChild<QLabel*>("arenaValue");
    generalLabels_.battlesWon = this->findChild<QLabel*>("battlesValue");
    generalLabels_.gold = this->findChild<QLabel*>("goldValue");
    generalLabels_.heroesAlive = this->findChild<QLabel*>("heroesValue");
}
