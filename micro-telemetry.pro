#-------------------------------------------------
#
# Project created by QtCreator 2017-03-17T07:45:52
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = micro-telemetry
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    model.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    model.h

FORMS    += mainwindow.ui
