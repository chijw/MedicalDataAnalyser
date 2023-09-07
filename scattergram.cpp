#include "scattergram.h"
#include <QVBoxLayout>
scattergram::scattergram(QWidget *parent)
    : QWidget{parent}
{
    chart = new QChart;
    chartView = new QChartView;
    //布局设计
    chartView->setRenderHint(QPainter::Antialiasing);
    QVBoxLayout * chartLayout = new QVBoxLayout;
    chartLayout->addWidget(chartView);

}
