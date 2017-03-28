#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QLayoutItem>
#include <QThread>
#include "qcustomplot.h"
#include "model.h"

#include "addmeasurementdialog.h"

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

    void on_btConnect_clicked();

    void on_btDisconnect_clicked();

    void on_btPlot0Add_clicked();

public slots:
    void onSerialPortsChanged();
    void notify();
    void newMeasurement(int measID);
    void onAddMeasurementDialogClosed(QVector<uint16_t> *mids);
    void onMeasurementChanged(uint16_t mid);

private:
    Ui::MainWindow *ui;
    Model *mModel;
    QSpacerItem *mSpacer;
    QThread *mModelThread;

    typedef struct
    {
        uint16_t mid;
        int graphid;
        QCPGraph *graph;
        QCustomPlot *plot;
    } tsMeasGrpahAssociation;
    QList<tsMeasGrpahAssociation*> *mMGAssociations;

    AddMeasurementDialog *mAddMeasDialog;
};

#endif // MAINWINDOW_H
