#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "csvdata.h"

#include "QtCharts/QChartView"
#include "QtCharts/QLineSeries"

#include <QDialog>
#include <QLayout>
#include <vector>

namespace Ui {
class Histogram;
}

class Histogram : public QWidget
{
    Q_OBJECT

public:
    explicit Histogram(int cursorColumnIndex, CsvData data,  QWidget *parent = nullptr);
    ~Histogram();
    void initHistogram();
    void drawNormalDistribution();
private:
    //存储图表
    float minValue;
    float maxValue;
    int col;
    QChart * chart;
    QChartView * chartView;
    //存储本列的所有数据
    std::vector<float>numList;
    //判断正态曲线是否被绘制
    bool isNormalDistributionDrawed;
    QLineSeries * normalDistribution;
};

#endif // HISTOGRAM_H

