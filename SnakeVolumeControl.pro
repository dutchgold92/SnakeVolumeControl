#-------------------------------------------------
#
# Project created by QtCreator 2017-06-09T18:39:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SnakeVolumeControl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    snake.cpp \
    snake_controller.cpp \
    volume_controller.cpp \
    snake_view.cpp

HEADERS  += mainwindow.h \
    snake.h \
    utils.h \
    snake_controller.h \
    volume_controller.h \
    snake_view.h

FORMS    += mainwindow.ui

CONFIG += c++11
