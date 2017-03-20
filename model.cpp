#include "model.h"
#include "mainwindow.h"

#include <QTimer>
#include <QSerialPortInfo>

Model::Model(QObject *parent) : QObject(parent)
{
    mPort = new QSerialPort(this);
    mPortList = new QStringList;
    mMeasurements = new QList<Measurement*>;

    connect(mPort, &QSerialPort::readyRead, this, &Model::readData);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    timer->start(1000);
}

void Model::timerSlot()
{
    QStringList *portList = new QStringList;
    qInfo("timerSlot");
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    // I am only interested in the port names
    for(int i = 0; i < ports.length(); i++)
    {
        portList->append(ports.at(i).portName());
    }
    qSort(*portList);

    // Check if changed
    if( (mPortList == NULL) || (*mPortList != *portList) )
    {
        mPortList = portList;
        emit serialPortsChanged();
    }

    // DEBUG: Add measurement
    Measurement *meas = new Measurement;
    meas->name = "Test Name";
    meas->unit = "Test unit";
    mMeasurements->append(meas);
    emit measurementAdded(mMeasurements->length()-1);
}

void Model::readData()
{
    QByteArray data = mPort->readAll();
//    console->putData(data);
}

QStringList* Model::getPortList()
{
    return this->mPortList;
}


QString Model::getMeasurementName(int measID)
{
//TODO implement
    return mMeasurements->at(measID)->name;
}

QString Model::getMeasurementUnit(int measID)
{
// TODO implement
    return mMeasurements->at(measID)->unit;
}
