#include <QGraphicsView>
#include <QPixmap>
#include <QGraphicsItem>
#include <QDebug>
#include <QMouseEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameview.h"
#include "game.h"

MainWindow::MainWindow(Engine *engine, GameWindowInterface *game,
                       int viewWidth, int viewHeight, QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    engine_(engine), game_(game),
    scene_(NULL), shopDialog_(NULL), aboutDialog_(NULL), manualDialog_(NULL), scoreDialog_(NULL),
    inventoryView_(NULL), gameInfoBox_(NULL),
    viewWidth_(viewWidth), viewHeight_(viewHeight)
{
    ui_->setupUi(this);

    // Allocate the score dialog right away for signal connecting
    scoreDialog_ = new ScoreDialog();

    // Connect signals and slots
    // Engine signals
    QObject::connect(engine_, SIGNAL(writeToLog(QString)), this, SLOT(writeToLog(QString)));
    // Game signals
    // Dynamic cast used to connect the signals through the interface
    QObject::connect(dynamic_cast<Game*>(game_), SIGNAL(writeToLog(QString)), this, SLOT(writeToLog(QString)));
    QObject::connect(dynamic_cast<Game*>(game_), SIGNAL(updateView(std::vector<World::change>)),
                     this, SLOT(updateView(std::vector<World::change>)));
    QObject::connect(dynamic_cast<Game*>(game_), SIGNAL(updateInventory()), this, SLOT(updateInventory()));
    QObject::connect(dynamic_cast<Game*>(game_), SIGNAL(updateEntityInfoView()), this, SLOT(updateEntityInfoView()));
    QObject::connect(dynamic_cast<Game*>(game_), SIGNAL(updateGameInfo()), this, SLOT(updateGameInfo()));
    QObject::connect(dynamic_cast<Game*>(game_), SIGNAL(changeMapVisibility(bool)),
                     this, SLOT(changeMapVisibility(bool)));
    QObject::connect(dynamic_cast<Game*>(game_), SIGNAL(gameEnded(unsigned int)), this, SLOT(endGame(unsigned int)));
    // Score screen signals
    QObject::connect(scoreDialog_, SIGNAL(scoreScreen_newGame()), this, SLOT(on_actionNewGame_triggered()));
    QObject::connect(scoreDialog_, SIGNAL(scoreScreen_exit()), this, SLOT(on_actionExit_triggered()));

    QObject::connect(dynamic_cast<Game*>(game_), SIGNAL (openShop()),this, SLOT(openShop()));

    initializeUi();

    initialize_ = true;
    // Initialize the Engine and Game
    engine_->run();
    // Open the ShopDialog at the start of the game
    openShopDialog();
}

MainWindow::~MainWindow()
{
    delete ui_;
    delete engine_;
    delete scene_;
    delete shopDialog_;
    delete aboutDialog_;
    delete scoreDialog_;
}

void MainWindow::on_actionNewGame_triggered()
{
    qDebug() << "Toolbar click: Game->New Game";

    // Clear the game log
    ui_->gameLog->clear();
    // Restart the game
    engine_->newGame();
}

void MainWindow::on_actionExit_triggered()
{
    qDebug() << "Toolbar click: Game->Exit";
    close();
}

void MainWindow::on_actionManual_triggered()
{
    qDebug() << "Toolbar click: Help->Manual";
    // Allocate a new dialog if one does not exist yet
    if (manualDialog_ == NULL)
        manualDialog_ = new ManualDialog();
    manualDialog_->setModal(true);
    manualDialog_->show();
}

void MainWindow::on_actionAbout_triggered()
{
    qDebug() << "Toolbar click: Help->About";
    // Allocate a new dialog if one does not exist yet
    if (aboutDialog_ == NULL)
        aboutDialog_ = new AboutDialog();
    aboutDialog_->setModal(true);
    aboutDialog_->show();
}

void MainWindow::writeToLog(QString msg)
{
    ui_->gameLog->insertHtml(msg);
}

// Last change on change vector contains iformation about new selected player.
void MainWindow::updateView(std::vector<World::change> changes)
{
    QList<QGraphicsItem*> itemsList = scene_->items(Qt::DescendingOrder);

    qDebug() << "Updating the game view";

    for(unsigned int i = 0; i < changes.size(); ++i)
    {
        if(changes[i].tile == NULL)
        {
            continue;
        }

        for(int j = 0; j < itemsList.size(); ++j)
        {
            if (itemsList.at(j)->pos() == QPointF(changes[i].x, changes[i].y ))
            {
                QGraphicsItem *a = itemsList.at(j);
                scene_->removeItem(a);
            }
        }

        // Ground drawing
        QPixmap tileImg = changes[i].tile->getTerrainImage();
        QGraphicsItem *tileGround = new QGraphicsPixmapItem(tileImg);
        tileGround->setPos(changes[i].x, changes[i].y);
        scene_->addItem(tileGround);

        // Entity drawing
        tileImg = changes[i].tile->getObjectImage();
        QGraphicsItem *tileEntity = new QGraphicsPixmapItem(tileImg);
        tileEntity->setPos(changes[i].x, changes[i].y);
        scene_->addItem(tileEntity);

        // Misc drawing
        tileImg = changes[i].tile->getMiscImage();
        QGraphicsItem *tileMisc = new QGraphicsPixmapItem(tileImg);
        tileMisc->setPos(changes[i].x, changes[i].y);
        scene_->addItem(tileMisc);

        // Allowed move drawing
        tileImg = changes[i].tile->getAllowedAction();
        QGraphicsItem *tileAction = new QGraphicsPixmapItem(tileImg);
        tileAction->setPos(changes[i].x, changes[i].y);
        scene_->addItem(tileAction);
    }

    // Set frame for active player
    if(changes.size() != 0)
    {
        if(changes.back().activePlayer == true)
        {
            // Select new player
            QPixmap tileImg = changes.back().tile->getSelection();
            QGraphicsItem *tileSelection = new QGraphicsPixmapItem(tileImg);
            tileSelection->setPos(changes.back().x, changes.back().y);
            scene_->addItem(tileSelection);
        }
    }

    if(initialize_){
    itemsList = scene_->items(Qt::DescendingOrder);

    for(int i = 0; i < itemsList.size(); ++i)
    {
        itemsList[i]->hide();
    }
    initialize_ = false;
    }
}

void MainWindow::changeMapVisibility(bool visible)
{
    QList<QGraphicsItem*> itemsList = scene_->items(Qt::DescendingOrder);

    for(int i = 0; i < itemsList.size(); ++i)
    {
        if(visible)
        {
            itemsList[i]->show();
        }
        else
        {
            itemsList[i]->hide();

        }
    }
}

void MainWindow::updateInventory()
{
    qDebug() << "Updating inventory view";
    inventoryView_->update();
}

void MainWindow::updateEntityInfoView()
{
    qDebug() << "Updating entity info box";
    entityInfoView_->update();
}

void MainWindow::updateGameInfo()
{
    qDebug() << "Updating game info box";
    gameInfoBox_->update();
}

void MainWindow::endGame(unsigned int battlesWon)
{
    qDebug() << "Game ended, battles won:" << battlesWon;

    scoreDialog_->setScore(battlesWon);
    scoreDialog_->setModal(true);
    scoreDialog_->show();
}

void MainWindow::openShop()
{
    openShopDialog();
}

void MainWindow::initializeUi()
{
    setWindowTitle("TIE-20200 Harjoitustyö");

    // Get easier reference to different views of the UI
    inventoryView_ = ui_->inventoryView;
    inventoryView_->setGame(dynamic_cast<Game*> (game_));
    gameInfoBox_ = ui_->gameInfoView;
    gameInfoBox_->setGame(dynamic_cast<Game*> (game_));
    entityInfoView_ = ui_->entityInfoView;
    entityInfoView_->setGame(dynamic_cast<Game*> (game_));

    initializeScene();
}

void MainWindow::initializeScene()
{
    scene_ = new QGraphicsScene(0, 0, viewWidth_, viewHeight_);
    ui_->gameView->setScene(scene_);
    ui_->gameView->setEngine(engine_);
}

void MainWindow::openShopDialog()
{
    // Allocate a new dialog if one does not exist yet
    if (shopDialog_ == NULL)
    {
        shopDialog_ = new ShopDialog(dynamic_cast<GameShopInterface*> (game_));
    }
    else
    {
        shopDialog_->openShop(dynamic_cast<GameShopInterface*> (game_));
    }

    shopDialog_->setModal(true);
    shopDialog_->show();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        qDebug() << "Pressed right button outside Scene! Position " << event->pos();
    }

    else if(event->button() == Qt::LeftButton)
    {
        qDebug() << "Pressed left button outside Scene! Position " << event->pos();
    }
}
