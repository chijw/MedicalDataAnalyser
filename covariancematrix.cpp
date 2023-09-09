#include "covariancematrix.h"
#include "ui_covariancematrix.h"
#include "colorbar.h"
//Eigen库
#include "covariance.hpp"
#include "Eigen/Dense"
#include <iostream>
#include <vector>

#include <QLabel>
#include <QSpacerItem>

CovarianceMatrix::CovarianceMatrix(std::vector<std::vector<float>>numLists, QStringList titles, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CovarianceMatrix)
{
    ui->setupUi(this);
    int size = numLists.size();
    //设置表格的标签
    QLabel * label = new QLabel("test");
    for (int i = 0; i < size; ++i)
    {
        QLabel * label = new QLabel(titles[i]);
        label->setAlignment(Qt::AlignCenter);
        label->setFixedWidth(100);
        QSpacerItem * frontSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        QSpacerItem * backSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        ui->horizontalLayout->addSpacerItem(frontSpacer);
        ui->horizontalLayout->addWidget(label);
        ui->horizontalLayout->addSpacerItem(backSpacer);
    }
    //设置表格规格和填充模式；设置单元格不可编辑、不可填充
    ui->matrix->setRowCount(size);
    ui->matrix->setColumnCount(size);
    ui->matrix->horizontalHeader()->setVisible(false);
    ui->matrix->verticalHeader()->setVisible(false);
    ui->matrix->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->matrix->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->matrix->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->matrix->setSelectionMode(QAbstractItemView::NoSelection);
    auto cov = getCovariance(numLists);
    float minValue = round(cov(0,0) * 100) / 100.0;
    float maxValue = round(cov(0,0) * 100) / 100.0;
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            //保留两位小数
            float val = round(cov(i, j) * 100) / 100.0;
            //更新最大、最小值
            minValue = std::min(minValue, val);
            maxValue = std::max(maxValue, val);
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(val));
            item->setTextAlignment(Qt::AlignCenter);
            ui->matrix->setItem(i, j, item);
        }
    }
    ColorBar * colorBar = new ColorBar(minValue, maxValue);
    ui->colorLayout->addWidget(colorBar);
}

CovarianceMatrix::~CovarianceMatrix()
{
    delete ui;
}
