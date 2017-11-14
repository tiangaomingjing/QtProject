#-------------------------------------------------
#
# Project created by QtCreator 2017-11-13T09:57:14
#
#-------------------------------------------------

QT       += core gui designer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
INCLUDEPATH += ../QtTreeManager

TARGET = Together
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    ../QtTreeManager/qttreemanager.cpp

HEADERS  += dialog.h \
    ../QtTreeManager/qttreemanager.h

FORMS    += dialog.ui
