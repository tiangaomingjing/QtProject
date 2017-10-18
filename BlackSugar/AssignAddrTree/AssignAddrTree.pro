#-------------------------------------------------
#
# Project created by QtCreator 2017-10-18T11:17:23
#
#-------------------------------------------------

QT       += core gui designer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
INCLUDEPATH+=../QtTreeManager

TARGET = AssignAddrTree
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../QtTreeManager/qttreemanager.cpp

HEADERS  += mainwindow.h \
    ../QtTreeManager/qttreemanager.h

FORMS    += mainwindow.ui
