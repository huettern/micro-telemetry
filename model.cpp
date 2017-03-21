#include "model.h"
#include "mainwindow.h"


#include <QTimer>
#include <QSerialPortInfo>

Model::Model(QObject *parent) : QObject(parent)
{
    mPort = new QSerialPort(this);
    mPortList = new QStringList;
    mMeasurements = new QList<Measurement*>;
    mCurrentPacket = new Packet;

    mIsReceivingPacket = false;

    connect(mPort, &QSerialPort::readyRead, this, &Model::readData);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    timer->start(1000);
}

/**
 * @brief Called periodically
 */
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

    // DEBUG: Create packate
    QByteArray ba;
    ba.resize(7);
    ba[0] = 0xff;
    ba[1] = 0xcc;
    ba[2] = 0x07;
    ba[3] = 0x00;
    ba[4] = 0x00;
    ba[5] = 0xcb;
    ba[6] = 0xfe;
    Packet::tPacketStatus stat =  mCurrentPacket->addData(ba);
}

/**
 * @brief Gets called if bytes are received over serial
 */
void Model::readData()
{
    QByteArray data = mPort->readAll();

    Packet::tPacketStatus stat =  mCurrentPacket->addData(data);

    switch (stat) {
    case Packet::PACKET_NONE:
        break;
    case Packet::PACKET_STARTED:
        break;
    case Packet::PACKET_FILLING:
        break;
    case Packet::PACKET_ENDED:
        mFinishedPacket = mCurrentPacket;
        mCurrentPacket = new Packet;
        data = mFinishedPacket->getRemainingData();
        if (data.length() != 0)
        {
            mCurrentPacket->addData(data);
        }
        // Process data here
        break;
    default:
        break;
    }
}

/**
 * @brief Returns list of available ports
 * @return available ports
 */
QStringList* Model::getPortList()
{
    return this->mPortList;
}

/**
 * @brief get the name of a measurement
 * @param measurement id
 * @return Name
 */
QString Model::getMeasurementName(int measID)
{
    return mMeasurements->at(measID)->name;
}

/**
 * @brief get the unit of a measurement
 * @param measurement id
 * @return Unit
 */
QString Model::getMeasurementUnit(int measID)
{
    return mMeasurements->at(measID)->unit;
}
