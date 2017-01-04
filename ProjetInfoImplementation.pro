#-------------------------------------------------
#
# Project created by QtCreator 2016-12-13T11:44:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjetInfoImplementation
TEMPLATE = app
CONFIG += console c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    sceneclickable.cpp

HEADERS  += mainwindow.h \
    sceneclickable.h

FORMS    += mainwindow.ui
