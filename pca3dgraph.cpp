#include "pca3dgraph.h"
#include "ui_pca3dgraph.h"
//Eigen库
#include "pca.hpp"
#include "Eigen/Dense"
#include <iostream>
#include <Q3DCamera>

PCA3DGraph::PCA3DGraph(const CsvData& input, const std::vector<int>& col, QWidget *parent) :
    QDialog(parent), data(input), columns(col),
    ui(new Ui::PCA3DGraph)
{
    ui->setupUi(this);
    scatter = new Q3DScatter;
    //从整体数据中筛选出选中的数据
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
    auto res = pca(originMatrix, 3);
    QScatter3DSeries * bScatterSeries = new QScatter3DSeries;
    bScatterSeries->setBaseColor(QColor(0,0,255,120));
    QScatter3DSeries * mScatterSeries = new QScatter3DSeries;
    mScatterSeries->setBaseColor(QColor(255,0,0,120));
    QScatterDataArray bDataArray;
    QScatterDataArray mDataArray;
    //初始化数轴边界
    float xMinValue = res(0,0), xMaxValue = res(0,0);
    float yMinValue = res(0,1), yMaxValue = res(0,1);
    float zMinValue = res(0,2), zMaxValue = res(0,2);
    for (int i = 0; i < data.AllPerson().size(); ++i)
    {
        QScatterDataItem item;
        item.setPosition(QVector3D(res(i,0),res(i,1),res(i,2)));
        if (!attr[i])
        {
            bDataArray.append(item);
        }
        else
        {
            mDataArray.append(item);
        }

        xMinValue = std::min(xMinValue, res(i, 0));
        xMaxValue = std::max(xMaxValue, res(i, 0));
        yMinValue = std::min(yMinValue, res(i, 1));
        yMaxValue = std::max(yMaxValue, res(i, 1));
        zMinValue = std::min(zMinValue, res(i, 2));
        zMaxValue = std::max(zMaxValue, res(i, 2));
    }
    //创建数轴并绑定
    float xSpace = (xMaxValue - xMinValue) / 10;
    float ySpace = (yMaxValue - yMinValue) / 10;
    float zSpace = (zMaxValue - zMinValue) / 10;
    QValue3DAxis * axisX = new QValue3DAxis;
    QValue3DAxis * axisY = new QValue3DAxis;
    QValue3DAxis * axisZ = new QValue3DAxis;
    axisX->setRange(xMinValue - xSpace, xMaxValue + xSpace);
    axisY->setRange(yMinValue - ySpace, yMaxValue + ySpace);
    axisZ->setRange(zMinValue - zSpace, zMaxValue + zSpace);
    scatter->setAxisX(axisX);
    scatter->setAxisY(axisY);
    scatter->setAxisZ(axisZ);
    bScatterSeries->dataProxy()->addItems(bDataArray);
    mScatterSeries->dataProxy()->addItems(mDataArray);
    //设置点的大小
    bScatterSeries->setItemSize(0.1);
    mScatterSeries->setItemSize(0.1);
    //scatterSeries加入scatter
    scatter->addSeries(bScatterSeries);
    scatter->addSeries(mScatterSeries);
    //布局设置
    QVBoxLayout *layout = new QVBoxLayout(this);
    QWidget * container = QWidget::createWindowContainer(scatter);
    layout->addWidget(container);
    setLayout(layout);
    setWindowTitle("3D降维图");
}

PCA3DGraph::~PCA3DGraph()
{
    delete ui;
}
