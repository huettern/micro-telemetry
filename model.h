#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QSerialPort>

#include <QMainWindow>
#include "mainwindow.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = 0);

private:
    QSerialPort *mPort;

private slots:
    void readData();
    void timerSlot();

signals:
    void changed();

public slots:
};

#endif // MODEL_H
