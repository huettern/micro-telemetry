#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <QObject>
#include <QVector>

#include "packet.h"

class Measurement : public QObject
{
    Q_OBJECT
public:
    explicit Measurement(QObject *parent = 0);

    uint16_t id;
    QString name;
    QString unit;

    Packet::tDataTypes xDataType;
    Packet::tDataTypes yDataType;

    QVector<double> xdata;
    QVector<double> ydata;

signals:

public slots:
};

#endif // MEASUREMENT_H
