#include "model.h"
#include "mainwindow.h"

#include <QTimer>
#include <QSerialPortInfo>

Model::Model(QObject *parent) : QObject(parent)
{
    mPort = new QSerialPort(this);
    connect(mPort, &QSerialPort::readyRead, this, &Model::readData);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    timer->start(1000);
}

void Model::timerSlot()
{
    qInfo("timerSlot");
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
}

void Model::readData()
{
    QByteArray data = mPort->readAll();
//    console->putData(data);
}
