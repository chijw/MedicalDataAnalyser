#include "curvegraph.h"
#include "ui_curvegraph.h"
//Eigen库
#include "Eigen/Dense"
#include "leastsquare.hpp"
#include <iostream>
#include <vector>

#include <QChart>
#include <QLineSeries>
#include <QInputDialog>
#include <QValueAxis>

CurveGraph::CurveGraph(std::vector<float> ListX, std::vector<float> ListY, QStringList titles, QWidget *parent) :
    QDialog(parent), numListX(ListX), numListY(ListY), ui(new Ui::CurveGraph)
{
    ui->setupUi(this);
    chart = new QChart;
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->spinBox->setRange(1,10);
    ui->spinBox->setValue(3);
    int inDegree = QInputDialog::getInt(nullptr,"设置","设置多项式阶数：",3,1,10);
    auto res = fitLeastSquareAndPR(numListX, numListY, inDegree);
    auto coefficientList = std::get<0>(res);
    float xMinValue = * std::min_element(numListX.begin(), numListX.end());
    float xMaxValue = * std::max_element(numListX.begin(), numListX.end());
    float yMinValue = * std::min_element(numListY.begin(), numListY.end());
    float yMaxValue = * std::max_element(numListY.begin(), numListY.end());
    QValueAxis * axisX = new QValueAxis;
    QValueAxis * axisY = new QValueAxis;
    QLineSeries * lineSeries = new QLineSeries;
    lineSeries->setName(QString("多项式阶数：%1").arg(inDegree));
    //加入51个点
    int density = 50;
    for (int i = 0; i <= density; ++i)
    {
        //计算点(x,y)
        float x = xMinValue + i * (xMaxValue - xMinValue) / density;
        float y = 0.0;
        for (int i = coefficientList.size() - 1; i >= 0; i--)
        {
            y = y * x + coefficientList[i];
        }
        lineSeries->append(x,y);
    }
    chart->addSeries(lineSeries);

    axisX->setRange(xMinValue / 2, xMaxValue + xMinValue / 2);
    axisY->setRange(yMinValue / 2, yMaxValue + yMinValue / 2);
    axisX->setTitleText(titles[0]);
    axisY->setTitleText(titles[1]);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    lineSeries->attachAxis(axisX);
    lineSeries->attachAxis(axisY);


    ui->chartLayout->addWidget(chartView);
    connect(ui->fitFunction,&QPushButton::clicked,this,&CurveGraph::setLevel);
}

CurveGraph::~CurveGraph()
{
    delete ui;
}
void CurveGraph::setLevel()
{
    int inDegree = ui->spinBox->value();
    auto res = fitLeastSquareAndPR(numListX, numListY, inDegree);
    auto coefficientList = std::get<0>(res);
    float xMinValue = * std::min_element(numListX.begin(), numListX.end());
    float xMaxValue = * std::max_element(numListX.begin(), numListX.end());
    QLineSeries * lineSeries = new QLineSeries;
    lineSeries->setName(QString("多项式阶数：%1").arg(inDegree));
    //加入51个点
    int density = 50;
    for (int i = 0; i <= density; ++i)
    {
        //计算点(x,y)
        double x = xMinValue + i * (xMaxValue - xMinValue) / density;
        double y = 0.0;
        for (int i = coefficientList.size() - 1; i >= 0; i--)
        {
            y = y * x + coefficientList[i];
        }
        lineSeries->append(x,y);
    }
    chart->addSeries(lineSeries);
}
