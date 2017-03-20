#include <QGridLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "measurementwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    mModel = new Model;
    connect(mModel, SIGNAL(changed()), this, SLOT(notify()));
    connect(mModel, SIGNAL(serialPortsChanged()), this, SLOT(onSerialPortsChanged()));
    connect(mModel, SIGNAL(measurementAdded(int)), this, SLOT(newMeasurement(int)));

    mSpacer = new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->wMeasurementContainer->layout()->addItem(mSpacer);

//    // DEBUG
////    layout->addWidget(new MeasurementWidget, 0,0,1,1,Qt::AlignCenter);

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
//    ui->customPlot->addGraph();
//    ui->customPlot->graph(0)->setData(x, y);
//    // give the axes some labels:
//    ui->customPlot->xAxis->setLabel("x");
//    ui->customPlot->yAxis->setLabel("y");
//    // set axes ranges, so we see all data:
//    ui->customPlot->xAxis->setRange(-1, 1);
//    ui->customPlot->yAxis->setRange(0, 1);
//    ui->customPlot->replot();
}

MainWindow::~MainWindow()
{
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

    ui->wMeasurementContainer->layout()->removeItem(mSpacer);
    ui->wMeasurementContainer->layout()->addWidget(meas);
    ui->wMeasurementContainer->layout()->addItem(mSpacer);
}
