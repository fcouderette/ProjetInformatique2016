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

#unix|win32: LIBS += -ltinyxml2

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/x86_64-linux-gnu/release/ -ltinyxml2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/x86_64-linux-gnu/debug/ -ltinyxml2
else:unix: LIBS += -L$$PWD/../../../../../../../usr/lib/x86_64-linux-gnu/ -ltinyxml2

INCLUDEPATH += $$PWD/../../../../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../../../../usr/lib/x86_64-linux-gnu
