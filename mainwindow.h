#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QLayoutItem>
#include <QThread>
#include "model.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

public slots:
    void onSerialPortsChanged();
    void notify();
    void newMeasurement(int measID);

private:
    Ui::MainWindow *ui;
    Model *mModel;
    QSpacerItem *mSpacer;
    QThread *mModelThread;
};

#endif // MAINWINDOW_H
