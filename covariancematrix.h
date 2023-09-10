#ifndef COVARIANCEMATRIX_H
#define COVARIANCEMATRIX_H
#include "colorbar.h"
#include "scalemark.h"
//Eigen库
#include "covariance.hpp"
#include "rowfeature.hpp"
#include "Eigen/Dense"
#include <iostream>
#include <vector>

#include <QDialog>
#include <vector>
#include <QStringList>
#include <QTableWidget>

namespace Ui {
class CovarianceMatrix;
}

class CovarianceMatrix : public QDialog
{
    Q_OBJECT

public:
    enum class MatrixState;
    explicit CovarianceMatrix(std::vector<std::vector<float>>numLists, QStringList titles, QWidget *parent = nullptr);
    ~CovarianceMatrix();
public slots:
    void changeState();
private:
    Ui::CovarianceMatrix *ui;
    int size;
    Eigen::MatrixXf cov;
    Eigen::MatrixXf cor;
    ColorBar * covColorBar;
    ColorBar * corColorBar;
    ScaleMark * scaleMark;
    //state用于记录是否为协方差矩阵（否则为相关系数矩阵）
    bool state;
    float covMinValue;
    float covMaxValue;
    float corMinValue;
    float corMaxValue;


};

#endif // COVARIANCEMATRIX_H
