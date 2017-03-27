#include "addmeasurementdialog.h"
#include "ui_addmeasurementdialog.h"

AddMeasurementDialog::AddMeasurementDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMeasurementDialog)
{
    ui->setupUi(this);
}

AddMeasurementDialog::~AddMeasurementDialog()
{
    delete ui;
}
