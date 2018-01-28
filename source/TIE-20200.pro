#-------------------------------------------------
#
# Project created by QtCreator 2015-08-27T14:27:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TIE-20200
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    engine.cpp \
    game.cpp \
    entity.cpp \
    terrain.cpp \
    item.cpp \
    tile.cpp \
    world.cpp \
    aboutdialog.cpp \
    gameview.cpp \
    hero.cpp \
    enemy.cpp \
    character.cpp \
    battlemanager.cpp \
    gamelog.cpp \
    shopdialog.cpp \
    inventory.cpp \
    misc.cpp \
    inventoryview.cpp \
    gameinfobox.cpp \
    entityinfoview.cpp \
    equipment.cpp \
    consumable.cpp \
    itemgenerator.cpp \
    basicai.cpp \
    manualdialog.cpp \
    scoredialog.cpp \
    shopitemlist.cpp

HEADERS  += mainwindow.h \
    engine.h \
    game.h \
    entity.h \
    terrain.h \
    item.h \
    tile.h \
    world.h \
    aboutdialog.h \
    character.h \
    hero.h \
    enemy.h \
    gameview.h \
    battlemanager.h \
    gamelog.h \
    shopdialog.h \
    gamewindowinterface.h \
    gameengineinterface.h \
    gameinfointerface.h \
    inventory.h \
    misc.h \
    inventoryview.h \
    gameinfobox.h \
    entityinfoview.h \
    equipment.h \
    consumable.h \
    helpers.h \
    itemgenerator.h \
    gameshopinterface.h \
    ai.h \
    basicai.h \
    manualdialog.h \
    scoredialog.h \
    shopitemlist.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    shopdialog.ui \
    manualdialog.ui \
    scoredialog.ui

RESOURCES += \
    images.qrc

CONFIG += c++11
