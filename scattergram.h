#ifndef SCATTERGRAM_H
#define SCATTERGRAM_H

#include "QtCharts/QChartView"
#include <QWidget>

class scattergram : public QWidget
{
    Q_OBJECT
public:
    explicit scattergram(QWidget *parent = nullptr);
private:
    QChart * chart;
    QChartView * chartView;
};

#endif // SCATTERGRAM_H
