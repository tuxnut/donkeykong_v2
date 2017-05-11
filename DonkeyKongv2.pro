#-------------------------------------------------
#
# Project created by QtCreator 2017-04-03T18:29:35
#
#-------------------------------------------------

QT       += core gui uitools multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DonkeyKongv2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        view.cpp \
    coregame.cpp \
    model.cpp \
    block.cpp \
    player.cpp \
    banana.cpp \
    paddle.cpp

HEADERS  += view.h \
    coregame.h \
    model.h \
    block.h \
    player.h \
    banana.h \
    gamesettings.h \
    paddle.h

FORMS    += view.ui \
    highscore.ui

RESOURCES += \
    res.qrc

DISTFILES += \
    ToDo
