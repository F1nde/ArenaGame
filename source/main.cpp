#include <QApplication>
#include <QtGlobal>
#include <QTime>

#include "mainwindow.h"
#include "engine.h"
#include "game.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialize qsrand with current time.
    // This allows qrand() to be used to generate
    // pseudorandom numbers through the application.
    qsrand(QTime::currentTime().msec());

    // Create Game and Engine
    // Give ownership of the game to the Engine (controller)
    Game *g = new Game();
    Engine *e = new Engine(g);

    // Create MainWindow and give references to Engine (controller)
    // and Game (model)
    MainWindow w(e, g, 640, 480);
    w.show();

    return a.exec();
}
