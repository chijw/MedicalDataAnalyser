#include "pca2dgraph.h"
#include "ui_pca2dgraph.h"
#include "newtableview.h"
//Eigen库
#include "Eigen/Dense"
#include "pca.hpp"
#include <iostream>

#include <QScatterSeries>
#include <QValueAxis>
#include <QBrush>

PCA2DGraph::PCA2DGraph(const CsvData& input, const std::vector<int>& col, QWidget *parent) :
    QDialog(parent), data(input), columns(col), ui(new Ui::PCA2DGraph)
{
    ui->setupUi(this);
    std::vector<std::vector<float>> originMatrix;
    std::vector<bool> attr;
    for (const auto & p : data.AllPerson())
    {
        std::vector<float> originDot;
        for (int idx : col)
        {
            if (idx != 1)
            {
                originDot.emplace_back(p.Data().at(idx - 2));
            }
            else
            {
                originDot.emplace_back(data.MapList()[p.Diagnosis()]);
            }
        }
        originMatrix.emplace_back(originDot);
        attr.emplace_back(data.MapList()[p.Diagnosis()]);
    }
    auto res = pca(originMatrix, 2);
    std::vector<QScatterSeries *> seriesList;
    int k = 0;
    for (int i = 0; i < data.TypeList().size(); ++i)
    {
        k = std::max(k, data.TypeList().at(i));
    }
    k += 1;
    for (int i = 0; i < k; ++i)
    {
        QScatterSeries * series = new QScatterSeries;
        series->setColor(NewTableView::colorList[i]);
        series->setMarkerSize(8);
        seriesList.emplace_back(series);
    }
    if (!data.IsClustered())
    {
        seriesList[0]->setName("benign");
        seriesList[1]->setName("malignant");
    }
    //求x轴、y轴的上下界
    float xMinValue = res(0,0), xMaxValue = res(0,0);
    float yMinValue = res(0,1), yMaxValue = res(0,1);
    for (int i = 0; i < data.TypeList().size(); ++i)
    {
        seriesList[data.TypeList()[i]]->append(res(i, 0), res(i, 1));
        xMinValue = std::min(res(i,0),xMinValue);
        xMaxValue = std::max(res(i,0),xMaxValue);
        yMinValue = std::min(res(i,1),yMinValue);
        yMaxValue = std::max(res(i,1),yMaxValue);
    }
    //初始化信息展示标签
    posLabel = new QLabel(this);
    posLabel->setStyleSheet("background-color : lightgray");
    //初始化chart, chartView等组件
    chart = new QChart;
    for (const auto & series : seriesList)
    {
        chart->addSeries(series);
    }
    QValueAxis * axisX = new QValueAxis;
    QValueAxis * axisY = new QValueAxis;
    //扩充一定的显示范围
    float xSpace = (xMaxValue - xMinValue) / 10;
    float ySpace = (yMaxValue - yMinValue) / 10;
    axisX->setRange(xMinValue - xSpace, xMaxValue + xSpace);
    axisY->setRange(yMinValue - ySpace, yMaxValue + ySpace);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    for (const auto & series : seriesList)
    {
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }
    chartView = new QChartView(chart);
    ui->chartLayout->addWidget(chartView);
    for (const auto & series : seriesList)
    {
        connect(series, &QScatterSeries::hovered, this, &PCA2DGraph::handlePointHovered);
    }
}

PCA2DGraph::~PCA2DGraph()
{
    delete ui;
}
void PCA2DGraph::handlePointHovered(QPointF point, bool state)
{
    if (state)
    {
        //TODO
        QString text = QString("(%1,%2)").arg(point.x()).arg(point.y());
        for (int idx : columns)
        {
            if (idx != 1)
            {

            }
        }
        posLabel->setText(text);
        QPoint curPos = mapFromGlobal(QCursor::pos());
        posLabel->move(curPos.x() - posLabel->width() / 2, curPos.y() - posLabel->height() * 1.5);
        posLabel->show();
    }
    else
    {
        posLabel->hide();
    }
}
