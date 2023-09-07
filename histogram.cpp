#include "histogram.h"
#include "ui_histogram.h"
//qtCharts库
#include "QtCharts/QChartView"
#include "QtCharts/QBarSeries"
#include "QtCharts/QBarSet"
#include "Qtcharts/QBarCategoryAxis"
#include "QtCharts/QValueAxis"

#include <QInputDialog>
Histogram::Histogram(int cursorColumnIndex, CsvData data, QLayout * parentLayout, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Histogram)
{
    ui->setupUi(this);
    if (cursorColumnIndex != -1)
    {
        //创建图表和视图
        QChart * chart = new QChart;
        QChartView * chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        QBarSeries * histogramSeries = new QBarSeries;
        QBarSet * dataSet = new QBarSet("Frequency");
        int col;

        if (cursorColumnIndex != 1)
        {
            QVector<float>numList;
            for (const auto & p : data.AllPerson())
            {
                numList.emplaceBack(p.Data().at(cursorColumnIndex - 1));
            }
            col = QInputDialog::getInt(nullptr,"设置","设置列数（10-50）：",20,10,50);
            float minValue = * std::min_element(numList.begin(),numList.end());
            float maxValue = * std::max_element(numList.begin(),numList.end());
            float width = ( maxValue - minValue ) / col;
            QVector<int>frequencyCnt(col,0);
            for (const float & value : numList)
            {
                int idx = static_cast<int>((value - minValue) / width);
                if (idx >= 0 && idx < col)
                {
                    frequencyCnt[idx]++;
                }
            }
            //将数据加入到dataSet
            for (int i = 0; i < col; ++i)
            {
                * dataSet << frequencyCnt[i];
            }
            //将数据添加到chart
            histogramSeries->append(dataSet);
            chart->addSeries(histogramSeries);
            //创建x，y轴
            QBarCategoryAxis *axisX = new QBarCategoryAxis;
            for (int i = 0; i < col; ++i)
            {
                float colStart = minValue + i * width;
                float colEnd = colStart + width;
                // 使用区间作为X轴标签
                axisX->append(QString("%1-%2").arg(colStart).arg(colEnd));
            }
            chart->addAxis(axisX, Qt::AlignBottom);
            histogramSeries->attachAxis(axisX);
            QValueAxis *axisY = new QValueAxis;
            chart->addAxis(axisY, Qt::AlignLeft);
            histogramSeries->attachAxis(axisY);
            //设置图表和视图的属性
            histogramSeries->setBarWidth(1);
            chart->setTitle("Frequency Distribution Histogram");
            chartView->setRenderHint(QPainter::Antialiasing);
            parentLayout->addWidget(chartView);
        }
        else
        {
            col = 2;
            QBarSet * dataSet = new QBarSet("Frequency");
            QVector<int>frequencyCnt(col,0);
            for (const auto & p : data.AllPerson())
            {
                frequencyCnt[data.MapList()[p.Diagnosis()]]++;
            }
            for (int i = 0; i < col; ++i)
            {
                * dataSet << frequencyCnt[i];
            }
            histogramSeries->append(dataSet);
            chart->addSeries(histogramSeries);
            QBarCategoryAxis *axisX = new QBarCategoryAxis;
            for (int i = 0; i < col; ++i)
            {
                axisX->append(QString::number(i));
            }
            chart->addAxis(axisX, Qt::AlignBottom);
            histogramSeries->attachAxis(axisX);
            QValueAxis *axisY = new QValueAxis;
            chart->addAxis(axisY, Qt::AlignLeft);
            histogramSeries->attachAxis(axisY);
            //设置图表和视图的属性
            histogramSeries->setBarWidth(0.4);
            chart->setTitle("Frequency Distribution Histogram");
            chartView->setRenderHint(QPainter::Antialiasing);
            parentLayout->addWidget(chartView);
        }
    }
}

Histogram::~Histogram()
{
    delete ui;
}
