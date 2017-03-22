#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QSerialPort>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "measurement.h"
#include "packet.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = 0);

    void setThread (QThread *thr);
    void abortThread();

    QStringList* getPortList();
    QString getMeasurementName(int measID);
    QString getMeasurementUnit(int measID);

private:
    QSerialPort *mPort;
    QStringList *mPortList;
    QList<Measurement*> *mMeasurements;

    Packet *mCurrentPacket;
    Packet *mFinishedPacket;

    bool mIsReceivingPacket;
    bool mAbort = false;

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
