#ifndef CURVEGRAPH_H
#define CURVEGRAPH_H

#include "QtCharts/QChartView"
#include <QDialog>
#include <vector>
#include <QStringList>

namespace Ui {
class CurveGraph;
}

class CurveGraph : public QDialog
{
    Q_OBJECT

public:
    explicit CurveGraph(const std::vector<float>& ListX,const std::vector<float>& ListY,
                        const QStringList& titles, QWidget *parent = nullptr);
    ~CurveGraph();
public slots:
    void setLevel();
private:
    Ui::CurveGraph *ui;
    std::vector<float> numListX;
    std::vector<float> numListY;
    QChart * chart;
    QChartView * chartView;
};

#endif // CURVEGRAPH_H
