#ifndef PCA2DGRAPH_H
#define PCA2DGRAPH_H

#include "csvdata.h"
#include <QDialog>
#include <vector>

namespace Ui {
class PCA2DGraph;
}

class PCA2DGraph : public QDialog
{
    Q_OBJECT

public:
    explicit PCA2DGraph(const CsvData& input, const std::vector<int>& col, QWidget *parent = nullptr);
    ~PCA2DGraph();

private:
    Ui::PCA2DGraph *ui;
    CsvData data;
    std::vector<int> columns;
};

#endif // PCA2DGRAPH_H
