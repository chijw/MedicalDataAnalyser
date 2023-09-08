#ifndef SCATTERGRAM_H
#define SCATTERGRAM_H

#include "QtCharts/QChartView"
#include "csvdata.h"
#include <QWidget>
#include <vector>

class Scattergram : public QWidget
{
    Q_OBJECT
public:
    explicit Scattergram(int idxX, int idxY, CsvData data, QWidget *parent = nullptr);
    void initScattergram();
private:
    QChart * chart;
    QChartView * chartView;
    std::vector<float> numListX;
    std::vector<float> numListY;
};

#endif // SCATTERGRAM_H
