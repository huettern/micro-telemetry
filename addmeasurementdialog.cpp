

#include <qdebug.h>

#include "addmeasurementdialog.h"
#include "ui_addmeasurementdialog.h"

AddMeasurementDialog::AddMeasurementDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMeasurementDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(accepted()),this,SLOT(okClick()));
    connect(ui->buttonBox, SIGNAL(rejected()),this,SLOT(cancelClick()));


    mModel = new QStringListModel(this);
    mList = new QStringList();
}

AddMeasurementDialog::~AddMeasurementDialog()
{
    delete ui;
}

void AddMeasurementDialog::addEntry(QString *name)
{
    mList->append(name->toUtf8());
}

void AddMeasurementDialog::show()
{
    mList->sort();
    mModel->setStringList(*mList);
    ui->listView->setModel(mModel);
    QDialog::show();
}

void AddMeasurementDialog::okClick()
{
//    qInfo("Dialog: %d", ui->listView->selectedIndexes().at(0));
//    ui->listView->selectedIndexes()

    QModelIndexList list =ui->listView->selectionModel()->selectedIndexes();

   QStringList slist;
   QVector<uint16_t> *mids = new QVector<uint16_t>;
   foreach(const QModelIndex &index, list){
       slist.append( index.data(Qt::DisplayRole ).toString());
       mids->append(index.data(Qt::DisplayRole ).toString().left(
                       index.data(Qt::DisplayRole ).toString().indexOf(":")).toUInt());
   }
   qDebug() << slist.join(",");
   emit dialogClosed(mids);
}

void AddMeasurementDialog::cancelClick()
{

}
