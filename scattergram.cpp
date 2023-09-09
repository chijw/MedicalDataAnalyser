#include "scattergram.h"

#include <QVBoxLayout>
#include <QLineSeries>
#include <QPushButton>
#include <QValueAxis>
#include <QInputDialog>
#include <iostream>
#include <QEvent>
#include <QMouseEvent>
#include <QToolTip>

//布局处理
void Scattergram::initScattergram()
{   
    chart->setTitle("散点图");
    chartView->setRenderHint(QPainter::Antialiasing);
    QHBoxLayout * chartLayout = new QHBoxLayout;
    chartLayout->addWidget(chartView);
    //设置按钮布局
    QHBoxLayout * buttonLayout = new QHBoxLayout;
    QSpacerItem * spacer = new QSpacerItem(400, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);
    buttonLayout->addItem(spacer);
    QPushButton * closeBtn = new QPushButton;
    closeBtn->setText("关闭");
    buttonLayout->addWidget(closeBtn);
    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(chartLayout);
    mainLayout->addLayout(buttonLayout);
    connect(closeBtn, &QPushButton::clicked, [=](){
        delete this;
    });

}
Scattergram::Scattergram(std::vector<float> listX, std::vector<float> listY, QStringList titles, QWidget *parent)
    : QWidget{parent}, numListX(listX), numListY(listY)
{
    scatterSeries = new QScatterSeries;
    for (int i = 0; i < numListX.size(); ++i)
    {
        scatterSeries->append(numListX[i],numListY[i]);
    }
    chart = new QChart;
    chart->addSeries(scatterSeries);
    QValueAxis * axisX = new QValueAxis;
    QValueAxis * axisY = new QValueAxis;
    float xMinValue = * std::min_element(numListX.begin(), numListX.end());
    float xMaxValue = * std::max_element(numListX.begin(), numListX.end());
    float yMinValue = * std::min_element(numListY.begin(), numListY.end());
    float yMaxValue = * std::max_element(numListY.begin(), numListY.end());
    //扩充一定的显示范围
    axisX->setRange(xMinValue / 2, xMaxValue + xMinValue / 2);
    axisY->setRange(yMinValue / 2, yMaxValue + yMinValue / 2);
    axisX->setTitleText(titles[0]);
    axisY->setTitleText(titles[1]);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    scatterSeries->attachAxis(axisX);
    scatterSeries->attachAxis(axisY);
    chartView = new QChartView(chart);
    scatterSeries->setMarkerSize(8);
    initScattergram();
}

