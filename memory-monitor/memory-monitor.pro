#-------------------------------------------------
#
# Project created by QtCreator 2017-05-22T11:20:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = memory-monitor
TEMPLATE = app


SOURCES += main.cpp\
        memorymonitor.cpp \
        qcustomplot.cpp

HEADERS  += memorymonitor.h \
            qcustomplot.h

FORMS    += memorymonitor.ui
