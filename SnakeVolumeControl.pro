#-------------------------------------------------
#
# Project created by QtCreator 2017-06-09T18:39:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Serpentine
TEMPLATE = app
DESTDIR = bin
OBJECTS_DIR = bin/build/objects
MOC_DOR = bin/build/moc
RCC_DIR = bin/build/rcc
UI_DIR = bin/build/ui

SOURCES += src/main/main.cpp\
        src/main/mainwindow.cpp \
    src/main/snake.cpp \
    src/main/snake_controller.cpp \
    src/main/volume_controller.cpp \
    src/main/snake_view.cpp

HEADERS  += include/mainwindow.h \
    include/snake.h \
    include/utils.h \
    include/snake_controller.h \
    include/volume_controller.h \
    include/snake_view.h \
    include/config.h

FORMS    += resources/ui/forms/mainwindow.ui

CONFIG += c++11

#RESOURCES += \
#    resources/ui/qrc/images.qrc
