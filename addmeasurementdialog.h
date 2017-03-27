#ifndef ADDMEASUREMENTDIALOG_H
#define ADDMEASUREMENTDIALOG_H

#include <QDialog>

namespace Ui {
class AddMeasurementDialog;
}

class AddMeasurementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMeasurementDialog(QWidget *parent = 0);
    ~AddMeasurementDialog();

private:
    Ui::AddMeasurementDialog *ui;
};

#endif // ADDMEASUREMENTDIALOG_H
