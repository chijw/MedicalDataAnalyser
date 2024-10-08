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
#include <QtWidgets/QGraphicsTextItem>
#include <iostream>
#include <algorithm>
#include <QCursor>

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
Scattergram::Scattergram(const std::vector<float>& listX, const  std::vector<float>& listY,
                         const QStringList& titles, QWidget *parent)
    : QWidget{parent}, numListX(listX), numListY(listY)
{
    std::vector<std::pair<float,float>> pointList;
    scatterSeries = new QScatterSeries;
    for (int i = 0; i < numListX.size(); ++i)
    {
        scatterSeries->append(numListX[i],numListY[i]);
        pointList.emplace_back(std::make_pair(numListX[i],numListY[i]));
    }
    std::sort(pointList.begin(), pointList.end(), [](std::pair<float,float> a, std::pair<float,float> b){
        if (a.first > b.first)
        {
            return true;
        }
        else if (a.first < b.first)
        {
            return false;
        }
        else
        {
            return a.second > b.second;
        }
    });
    chart = new QChart;
    chart->addSeries(scatterSeries);
    QValueAxis * axisX = new QValueAxis;
    QValueAxis * axisY = new QValueAxis;
    float xMinValue = * std::min_element(numListX.begin(), numListX.end());
    float xMaxValue = * std::max_element(numListX.begin(), numListX.end());
    float yMinValue = * std::min_element(numListY.begin(), numListY.end());
    float yMaxValue = * std::max_element(numListY.begin(), numListY.end());
    //扩充一定的显示范围
    float xSpace = (xMaxValue - xMinValue) / 10;
    float ySpace = (yMaxValue - yMinValue) / 10;
    axisX->setRange(xMinValue - xSpace, xMaxValue + xSpace);
    axisY->setRange(yMinValue - ySpace, yMaxValue + ySpace);
    axisX->setTitleText(titles[0]);
    axisY->setTitleText(titles[1]);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    scatterSeries->attachAxis(axisX);
    scatterSeries->attachAxis(axisY);
    chartView = new QChartView(chart);
    scatterSeries->setMarkerSize(8);
    //创建存储重叠点的点列
    std::vector<std::pair<float,float>>overlapPointList;
    std::vector<int>pointCntList;
    int sum = 0;
    for (int i = 1; i < pointList.size(); ++i)
    {
        if (pointList[i] != pointList[i - 1])
        {
            continue;
        }
        int cnt = 1;
        while((i < pointList.size())&&(pointList[i] == pointList[i - 1]))
        {
            ++cnt;
            ++i;
        }
        overlapPointList.emplace_back(pointList[i - 1]);
        sum += cnt;
        pointCntList.emplace_back(cnt);
    }
    QString textContent(QString("共有 %1 个点重合\n").arg(sum));
    for (int i = 0; i < overlapPointList.size(); ++i)
    {
        textContent += QString("(%1,%2):%3\n")
           .arg(overlapPointList[i].first).arg(overlapPointList[i].second).arg(pointCntList[i]);
    }
    QLabel * label = new QLabel(chartView);
    label->setStyleSheet("color: black; font-size: 10px;");
    label->setText(textContent);
    //显示位置挪到左上角
    label->move(10, 10);
    posLabel = new QLabel(chartView);
    //鼠标悬停时显示信息
    connect(scatterSeries, &QScatterSeries::hovered, this, &Scattergram::handlePointHovered);
    initScattergram();
}

void Scattergram::handlePointHovered(QPointF point, bool state)
{
    if (state)
    {
        posLabel->setText(QString("(%1,%2)").arg(point.x()).arg(point.y()));
        posLabel->move(chartView->width() - posLabel->width() - 20, 20);
        posLabel->show();
    }
    else
    {
        posLabel->hide();
    }
}
