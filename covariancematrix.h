#ifndef COVARIANCEMATRIX_H
#define COVARIANCEMATRIX_H

#include <QDialog>
#include <vector>
#include <QStringList>

namespace Ui {
class CovarianceMatrix;
}

class CovarianceMatrix : public QDialog
{
    Q_OBJECT

public:
    explicit CovarianceMatrix(std::vector<std::vector<float>>numLists, QStringList titles, QWidget *parent = nullptr);
    ~CovarianceMatrix();

private:
    Ui::CovarianceMatrix *ui;
};

#endif // COVARIANCEMATRIX_H
