#-------------------------------------------------
#
# Project created by QtCreator 2017-10-19T15:00:36
#
#-------------------------------------------------


QT       += core gui designer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
INCLUDEPATH+=../QtTreeManager


TARGET = FillLimit
TEMPLATE = app


SOURCES += main.cpp\
        fillwindow.cpp \
    ../QtTreeManager/qttreemanager.cpp \
    filldialog.cpp

HEADERS  += fillwindow.h \
    ../QtTreeManager/qttreemanager.h \
    filldialog.h

FORMS    += fillwindow.ui \
    filldialog.ui

