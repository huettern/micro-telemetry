#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QSerialPort>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>


//#include "packet.h"
class Packet;
class Measurement;

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = 0);

    void setThread (QThread *thr);
    void abortThread();

    QStringList* getPortList();
    void openPort(QString* name);
    void closePort();
    QString getMeasurementName(int measID);
    QString getMeasurementUnit(int measID);
    Measurement* getMeasurement(int measID);
    void setClientRunning(bool s);
    void addMeasurement(Measurement* meas);
    void measurementChanged(uint16_t mid);

    QList<uint16_t> *getMeasurementIDs();

private:
    QSerialPort *mPort;
    QStringList *mPortList;
    QList<Measurement*> *mMeasurements;

    Packet *mCurrentPacket;
    Packet *mFinishedPacket;

    bool mIsReceivingPacket;
    bool mAbort = false;
    bool mClientRunning = false;

    QThread *mThread;
    QMutex mSyncMutex;
    QWaitCondition mParsePacket;


private slots:
    void readData();
    void timerSlot();

signals:
    void changed();
    void serialPortsChanged();
    void measurementAdded(int measID);

public slots:
    void do_continuousConversion();


};

#endif // MODEL_H
