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
    model.cpp \
    measurementwidget.cpp \
    measurement.cpp \
    packet.cpp \
    addmeasurementdialog.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    model.h \
    measurementwidget.h \
    measurement.h \
    packet.h \
    addmeasurementdialog.h

FORMS    += mainwindow.ui \
    addmeasurementdialog.ui
