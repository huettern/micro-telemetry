#include <QGridLayout>

#include "measurementwidget.h"


MeasurementWidget::MeasurementWidget(QWidget *parent) : QWidget(parent)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);

    setAutoFillBackground(true);
    setPalette(pal);

    QGridLayout* layout = new QGridLayout;

    layout->addWidget(&mlbName, 0, 1, 1, 1, Qt::AlignLeft);
    layout->addWidget(&mlbUnit, 1, 1, 1, 1, Qt::AlignLeft);

    setLayout(layout);


    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    show();

    // debug
    mlbName.setText("mlbName");
    mlbUnit.setText("mlbUnit");
}

void MeasurementWidget::setName(QString name)
{
    mlbName.setText(name);
}

void MeasurementWidget::setUnit(QString unit)
{
    mlbUnit.setText(unit);
}
