#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <vector>

#include "engine.h"
#include "gamewindowinterface.h"
#include "shopdialog.h"
#include "aboutdialog.h"
#include "manualdialog.h"
#include "scoredialog.h"
#include "inventoryview.h"
#include "entityinfoview.h"
#include "gameinfobox.h"
#include "tile.h"
#include "world.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Engine *engine, GameWindowInterface *game,
                        int viewWidth, int viewHeight, QWidget *parent = 0);
    ~MainWindow();

private slots:
    // Slots for menu options
    void on_actionNewGame_triggered();
    void on_actionExit_triggered();
    void on_actionManual_triggered();
    void on_actionAbout_triggered();

    // Slots for signals emitted from Game
    void writeToLog(QString msg);
    void updateView(std::vector<World::change> changes);
    void changeMapVisibility(bool visible);
    void updateInventory();
    void updateEntityInfoView();
    void updateGameInfo();
    void endGame(unsigned int battlesWon);
    void openShop();

private:
    // Initializes UI
    void initializeUi();
    // Initializes scene to be shown in the GameView
    void initializeScene();
    // Opens ShopDialog and allocates it is one does not exist
    void openShopDialog();
    // Overwritten for mouse events
    void mousePressEvent(QMouseEvent *event);

    Ui::MainWindow *ui_;

    Engine *engine_;
    GameWindowInterface *game_;

    QGraphicsScene *scene_;
    ShopDialog *shopDialog_;
    AboutDialog *aboutDialog_;
    ManualDialog *manualDialog_;
    ScoreDialog *scoreDialog_;
    InventoryView *inventoryView_;
    GameInfoBox *gameInfoBox_;
    EntityInfoView *entityInfoView_;

    int viewWidth_, viewHeight_;
    bool initialize_;
};

#endif // MAINWINDOW_H
