#ifndef ADDMEASUREMENTDIALOG_H
#define ADDMEASUREMENTDIALOG_H

#include <QDialog>
#include <QStringListModel>
#include <QStringList>

namespace Ui {
class AddMeasurementDialog;
}

class AddMeasurementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMeasurementDialog(QWidget *parent = 0);
    ~AddMeasurementDialog();

    void addEntry(QString *name);
    void show();

public slots:
    void okClick();
    void cancelClick();

signals:
    void dialogClosed(QVector<uint16_t> *mids);

private:
    Ui::AddMeasurementDialog *ui;

    QStringListModel *mModel;
    QStringList *mList;
};

#endif // ADDMEASUREMENTDIALOG_H
