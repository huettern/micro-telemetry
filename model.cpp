#include "model.h"
#include "mainwindow.h"
#include "packet.h"
#include "measurement.h"

#include <QTimer>
#include <QSerialPortInfo>
#include <qdebug.h>

Model::Model(QObject *parent) : QObject(parent)
{
    mPort = new QSerialPort(this);
    mPortList = new QStringList;
    mMeasurements = new QList<Measurement*>;
    mCurrentPacket = new Packet(this);

    mIsReceivingPacket = false;
    mSyncMutex.tryLock(100);

    connect(mPort, &QSerialPort::readyRead, this, &Model::readData);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    timer->start(1000);
}


void Model::setThread (QThread *thr)
{
    mThread = thr;
    connect(thr, SIGNAL(started()), this, SLOT(do_continuousConversion()));
}

void Model::do_continuousConversion()
{
    unsigned long timeUs = 100;
    while(mAbort == false)
    {
//        mThread->usleep(timeUs);
//        mSyncMutex.lock();
//        mParsePacket.wait(&mSyncMutex);
//        // A new packet is ready to be parsed

//        mSyncMutex.unlock();
    }
    qDebug() << "exiting FFT thread";
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

    // If a packet is parsed
    if(mParsePacket.wait(&mSyncMutex,1))
    {
        qInfo("packet ready to parse");
    }

    // DEBUG: Add measurement
//    Measurement *meas = new Measurement;
//    meas->name = "Test Name";
//    meas->unit = "Test unit";
//    mMeasurements->append(meas);
//    emit measurementAdded(mMeasurements->length()-1);

    // DEBUG: Create packate
//    QByteArray ba;
//    ba.resize(7);
//    ba[0] = 0xff;
//    ba[1] = 0xcc;
//    ba[2] = 0x07;
//    ba[3] = 0x00;
//    ba[4] = 0x00;
//    ba[5] = 0xcb;
//    ba[6] = 0xfe;
//    Packet::tPacketStatus stat =  mCurrentPacket->addData(ba);
}

/**
 * @brief Gets called if bytes are received over serial
 */
void Model::readData()
{
    qInfo("Data received");
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
        mCurrentPacket = new Packet(this);
        data = mFinishedPacket->getRemainingData();
        if (data.length() != 0)
        {
            mCurrentPacket->addData(data);
        }
        // parse packet
        mFinishedPacket->parse();
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

/**
 * @brief get the measurement
 * @param measurement id
 * @return Unit
 */
Measurement *Model::getMeasurement(int measID)
{
    return mMeasurements->at(measID);
}

/**
 * @brief Exits
 */
void Model::abortThread()
{
    mAbort = true;
}

void Model::openPort(QString *name)
{
    mPort->setPortName(*name);
    mPort->setBaudRate(115200);
    mPort->setDataBits(QSerialPort::Data8);
    mPort->setParity(QSerialPort::NoParity);
    mPort->setStopBits(QSerialPort::OneStop);
    mPort->setFlowControl(QSerialPort::NoFlowControl);
    try {
        mPort->open(QIODevice::ReadWrite);
    } catch (...) {
        qInfo("Error opening serial port");
    }
}

void Model::closePort()
{
    try {
        mPort->close();
    } catch (...) {
        qInfo("Error closing serial port");
    }
}

/**
 * @brief Model::setClientRunning
 * @param s
 */
void Model::setClientRunning(bool s)
{
    mClientRunning = s;
}

/**
 * @brief add new measurement
 * @param meas
 */
void Model::addMeasurement(Measurement* meas)
{
    mMeasurements->append(meas);
    emit measurementAdded(mMeasurements->length()-1);
}
