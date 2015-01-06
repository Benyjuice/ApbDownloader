#-------------------------------------------------
#
# Project created by QtCreator 2015-01-01T16:08:33
#
#-------------------------------------------------

QT       += core gui printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = apq
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    url.cpp

HEADERS  += mainwindow.h \
    url.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
