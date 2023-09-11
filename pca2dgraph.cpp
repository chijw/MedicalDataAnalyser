#include "pca2dgraph.h"
#include "ui_pca2dgraph.h"
//Eigen库
#include "Eigen/Dense"
#include "pca.hpp"
#include <iostream>

#include <QScatterSeries>
#include <QChart>
#include <QChartView>
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
    QScatterSeries * scatterSeries = new QScatterSeries;
    //求x轴、y轴的上下界
    float xMinValue = res(0,0), xMaxValue = res(0,0);
    float yMinValue = res(0,1), yMaxValue = res(0,1);
    QBrush BBrush(QColor(0,0,0)), MBrush(QColor(255,0,0));
    for (int i = 0; i < data.AllPerson().size(); ++i)
    {
        //TODO
        scatterSeries->append(res(i, 0), res(i, 1));

        xMinValue = std::min(res(i,0),xMinValue);
        xMaxValue = std::max(res(i,0),xMaxValue);
        yMinValue = std::min(res(i,1),yMinValue);
        yMaxValue = std::max(res(i,1),yMaxValue);
    }
    QChart * chart = new QChart;
    chart->addSeries(scatterSeries);
    QValueAxis * axisX = new QValueAxis;
    QValueAxis * axisY = new QValueAxis;
    axisX->setRange(xMinValue, xMaxValue);
    axisY->setRange(yMinValue, yMaxValue);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    scatterSeries->attachAxis(axisX);
    scatterSeries->attachAxis(axisY);
    QChartView * chartView = new QChartView(chart);
    ui->chartLayout->addWidget(chartView);
}

PCA2DGraph::~PCA2DGraph()
{
    delete ui;
}
