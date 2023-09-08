#include "histogram.h"
//qtCharts库
#include "QtCharts/QBarSeries"
#include "QtCharts/QBarSet"
#include "Qtcharts/QCategoryAxis"
#include "Qtcharts/QBarCategoryAxis"
#include "QtCharts/QValueAxis"

#include "Eigen/Dense"
#include "rowfeature.hpp"
#include <vector>
#include <iostream>

#include <QInputDialog>
#include <QLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <cmath>

void Histogram::initHistogram()
{
    //设置图表和视图的属性
    chart->setTitle("频率分布直方图");
    chartView->setRenderHint(QPainter::Antialiasing);
    QHBoxLayout * chartLayout = new QHBoxLayout;
    chartLayout->addWidget(chartView);
    //设置按钮布局
    QHBoxLayout * buttonLayout = new QHBoxLayout;
    QSpacerItem * spacer = new QSpacerItem(400, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);
    buttonLayout->addItem(spacer);
    QPushButton * normalDistribution = new QPushButton;
    normalDistribution->setText("正态曲线");
    buttonLayout->addWidget(normalDistribution);
    QPushButton * closeBtn = new QPushButton;
    closeBtn->setText("关闭");
    buttonLayout->addWidget(closeBtn);
    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(chartLayout);
    mainLayout->addLayout(buttonLayout);
    connect(closeBtn, &QPushButton::clicked, [=](){
        delete(this);
    });
    connect(normalDistribution,&QPushButton::clicked,this,&Histogram::drawNormalDistribution);

}
Histogram::Histogram(int cursorColumnIndex, CsvData data, QWidget *parent) :
    QWidget(parent), isNormalDistributionDrawed(false), normalDistribution(nullptr)
{
    if (cursorColumnIndex != -1 && cursorColumnIndex != 0)
    {
        //创建图表和视图
        chart = new QChart;
        chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        QBarSeries * histogramSeries = new QBarSeries;
        QBarSet * dataSet = new QBarSet("频率");
        //创建y轴
        QValueAxis * axisY = new QValueAxis;
        QVector<int>frequencyCnt;
        if (cursorColumnIndex != 1)
        {
            for (const auto & p : data.AllPerson())
            {
                numList.emplace_back(p.Data().at(cursorColumnIndex - 2));
            }
            col = QInputDialog::getInt(nullptr,"设置","设置列数（10-50）：",20,10,50,5);
            frequencyCnt = QVector<int>(col,0);
            minValue = * std::min_element(numList.begin(),numList.end());
            maxValue = * std::max_element(numList.begin(),numList.end());
            float width = ( maxValue - minValue ) / col;
            for (int i = 0; i < numList.size(); ++i)
            {
                int idx = static_cast<int>((numList[i] - minValue) / width);
                if (idx >= 0 && idx < col)
                {
                    frequencyCnt[idx]++;
                }
            }
            int labelNum = 5;
            float diff = (maxValue - minValue) / labelNum;
            QCategoryAxis * axisX = new QCategoryAxis;
            for (int i = 0; i <= labelNum; ++i)
            {
                float val = minValue + i * diff;
                axisX->append(QString::number(val), i * col / labelNum);
            }
            histogramSeries->setBarWidth(1);

            //将数据加入到dataSet
            for (int i = 0; i < col; ++i)
            {
                * dataSet << frequencyCnt[i];
            }
            //将数据添加到chart
            histogramSeries->append(dataSet);
            chart->addSeries(histogramSeries);
            chart->addAxis(axisX, Qt::AlignBottom);
            histogramSeries->attachAxis(axisX);
        }
        else
        {
            col = 2;
            minValue = 0;
            maxValue = 1;
            frequencyCnt = QVector<int>(col,0);
            for (const auto & p : data.AllPerson())
            {
                frequencyCnt[data.MapList()[p.Diagnosis()]]++;
                numList.emplace_back(data.MapList()[p.Diagnosis()]);
            }
            histogramSeries->setBarWidth(0.4);
            QBarCategoryAxis * axisX = new QBarCategoryAxis;
            axisX->append("B");
            axisX->append("M");
            //将数据加入到dataSet
            for (int i = 0; i < col; ++i)
            {
                * dataSet << frequencyCnt[i];
            }
            //将数据添加到chart
            histogramSeries->append(dataSet);
            chart->addSeries(histogramSeries);
            chart->addAxis(axisX, Qt::AlignBottom);
            histogramSeries->attachAxis(axisX);
        }
        chart->addAxis(axisY, Qt::AlignLeft);
        histogramSeries->attachAxis(axisY);
        initHistogram();
    }
}
Histogram::~Histogram()
{

}
void Histogram::drawNormalDistribution()
{
    if (!isNormalDistributionDrawed)
    {
        isNormalDistributionDrawed = true;
        std::tuple<float, float> AvgVar = getAvgVar(numList);
        float mean = std::get<0>(AvgVar);
        float stdDev = sqrt(std::get<1>(AvgVar));
        normalDistribution = new QLineSeries;
        normalDistribution->setName("正态分布");
        double x, y;
        int density = 50;
        float dx = (maxValue - minValue) / density;
        for (int i = 0; i <= density; ++i)
        {
            x = static_cast<double>(minValue + i * dx);
            y = (1 / (stdDev * sqrt(2 * M_PI))) * exp(-pow(x - mean, 2) / (2 * pow(stdDev, 2)));
            //由于表格中的x的值并非真实值，加入点的过程中需要对点进行映射
            float scaledX = col * (x - minValue) / (maxValue - minValue);
            normalDistribution->append(scaledX, y);
        }
        chart->addSeries(normalDistribution);
    }
    //若已经绘制了正太函数曲线，再次点击时删去曲线
    else
    {
        isNormalDistributionDrawed = false;
        chart->removeSeries(normalDistribution);
        delete normalDistribution;
        normalDistribution = nullptr;
    }
}
