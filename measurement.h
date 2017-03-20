#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <QObject>

class Measurement : public QObject
{
    Q_OBJECT
public:
    explicit Measurement(QObject *parent = 0);

    QString name;
    QString unit;

signals:

public slots:
};

#endif // MEASUREMENT_H
