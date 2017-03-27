#include <QGridLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "measurementwidget.h"
#include "measurement.h"

#include "addmeasurementdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->btDisconnect->setEnabled(false);
    mMGAssociations = new QList<tsMeasGrpahAssociation>;

    mModel = new Model();
//    mModelThread = new QThread();
//    mModel->setThread(mModelThread);
//    mModel->moveToThread(mModelThread);
//    mModelThread->start(QThread::NormalPriority);

    connect(mModel, SIGNAL(changed()), this, SLOT(notify()));
    connect(mModel, SIGNAL(serialPortsChanged()), this, SLOT(onSerialPortsChanged()));
    connect(mModel, SIGNAL(measurementAdded(int)), this, SLOT(newMeasurement(int)));

    mSpacer = new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->wMeasurementContainer->layout()->addItem(mSpacer);

//    // DEBUG
//    layout->addWidget(new MeasurementWidget, 0,0,1,1,Qt::AlignCenter);

//    mSpacer = new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
//    ui->wMeasurementContainer->layout()->addWidget(new MeasurementWidget);
//    ui->wMeasurementContainer->layout()->addWidget(new MeasurementWidget);
//    ui->wMeasurementContainer->layout()->addWidget(new MeasurementWidget);
//    ui->wMeasurementContainer->layout()->addWidget(new MeasurementWidget);
//    ui->wMeasurementContainer->layout()->addItem(mSpacer);
//    ui->wMeasurementContainer->layout()->removeItem(mSpacer);
//    ui->wMeasurementContainer->layout()->addWidget(new MeasurementWidget);
//    ui->wMeasurementContainer->layout()->addItem(mSpacer);


//    // generate some data:
//    QVector<double> x(101), y(101); // initialize with entries 0..100
//    for (int i=0; i<101; ++i)
//    {
//      x[i] = i/50.0 - 1; // x goes from -1 to 1
//      y[i] = x[i]*x[i]; // let's plot a quadratic function
//    }
//    // create graph and assign data to it:
    ui->customPlot->addGraph();
//    ui->customPlot->graph(0)->setData(x, y);
//    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
//    // set axes ranges, so we see all data:
//    ui->customPlot->xAxis->setRange(-1, 1);
//    ui->customPlot->yAxis->setRange(0, 1);
    ui->customPlot->replot();
}

MainWindow::~MainWindow()
{
    mModelThread->quit();
    mModelThread->wait();
    delete ui;
}

void MainWindow::notify()
{

}

void MainWindow::onSerialPortsChanged()
{

    QString currentSelection = ui->cbPortList->currentText();

    // regenerate the list
    ui->cbPortList->clear();
    for(int i = 0; i < mModel->getPortList()->length(); i++)
    {
        ui->cbPortList->addItem(mModel->getPortList()->at(i));
    }

    // try to reselect the previous selection
    int index = ui->cbPortList->findData(currentSelection);
    if ( index != -1 ) { // -1 for not found
       ui->cbPortList->setCurrentIndex(index);
    }

}

void MainWindow::newMeasurement(int measID)
{
    MeasurementWidget *meas = new MeasurementWidget;
    meas->setName(mModel->getMeasurementName(measID));
    meas->setUnit(mModel->getMeasurementUnit(measID));

    // Add Widget and the spacer after the widget
    ui->wMeasurementContainer->layout()->removeItem(mSpacer);
    ui->wMeasurementContainer->layout()->addWidget(meas);
    ui->wMeasurementContainer->layout()->addItem(mSpacer);
}

void MainWindow::on_btConnect_clicked()
{
    ui->cbPortList->setEnabled(false);
    ui->btConnect->setEnabled(false);
    ui->btDisconnect->setEnabled(true);
    QString name = ui->cbPortList->currentText();
    mModel->openPort(&name);
}

void MainWindow::on_btDisconnect_clicked()
{
    mModel->closePort();
    ui->cbPortList->setEnabled(true);
    ui->btConnect->setEnabled(true);
    ui->btDisconnect->setEnabled(false);
}

/**
 * @brief Update all Plots where the measurement is displayed
 * @param mid Measurement ID to update
 */
void MainWindow::measurementChanged(uint16_t mid)
{
    for(int i = 0; i < mMGAssociations->length(); i++)
    {
        if(mMGAssociations->at(i).mid == mid)
        {
            mMGAssociations->at(i).graph->setData(mModel->getMeasurement(mid)->xdata,
                                                 mModel->getMeasurement(mid)->ydata);
            mMGAssociations->at(i).plot->replot();
        }
    }
}

void MainWindow::on_btPlot0Add_clicked()
{
    AddMeasurementDialog *dialog = new AddMeasurementDialog();
    dialog->show();
}
