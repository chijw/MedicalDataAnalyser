#ifndef SCATTERGRAM_H
#define SCATTERGRAM_H

#include "QtCharts/QChartView"
#include <QWidget>
#include <vector>
#include <QScatterSeries>
#include <QLabel>

class Scattergram : public QWidget
{
    Q_OBJECT
public:
    explicit Scattergram(std::vector<float>listX, std::vector<float>listY, QStringList titles, QWidget *parent = nullptr);
    void initScattergram();
public slots:
    void handlePointHovered(QPointF, bool);
private:
    QScatterSeries * scatterSeries;
    QChart * chart;
    QChartView * chartView;
    QLabel * posLabel;
    std::vector<float> numListX;
    std::vector<float> numListY;
};

#endif // SCATTERGRAM_H
