#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QSerialPort>

#include "measurement.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = 0);

    QStringList* getPortList();
    QString getMeasurementName(int measID);
    QString getMeasurementUnit(int measID);

private:
    QSerialPort *mPort;
    QStringList *mPortList;
    QList<Measurement*> *mMeasurements;

private slots:
    void readData();
    void timerSlot();

signals:
    void changed();
    void serialPortsChanged();
    void measurementAdded(int measID);

public slots:
};

#endif // MODEL_H
