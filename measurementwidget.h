#ifndef MEASUREMENTWIDGET_H
#define MEASUREMENTWIDGET_H

#include <QWidget>
#include <QLabel>

class MeasurementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MeasurementWidget(QWidget *parent = 0);

    void setName(QString value);
    void setUnit(QString unit);
private:
    QLabel mlbName;
    QLabel mlbUnit;

signals:

public slots:
};

#endif // MEASUREMENTWIDGET_H
