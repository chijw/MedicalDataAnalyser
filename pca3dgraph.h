#ifndef PCA3DGRAPH_H
#define PCA3DGRAPH_H
#include "csvdata.h"

#include <QDialog>
#include <QtDataVisualization>
#include <vector>

namespace Ui {
class PCA3DGraph;
}

class PCA3DGraph : public QDialog
{
    Q_OBJECT

public:
    explicit PCA3DGraph(const CsvData& input, const std::vector<int>& col, QWidget *parent = nullptr);
    ~PCA3DGraph();

private:
    Ui::PCA3DGraph *ui;
    CsvData data;
    std::vector<int> columns;
    Q3DScatter *scatter;
};

#endif // PCA3DGRAPH_H
