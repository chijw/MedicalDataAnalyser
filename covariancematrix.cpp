#include "covariancematrix.h"
#include "ui_covariancematrix.h"
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
    //设置表格规格和填充模式；设置单元格不可编辑
    ui->matrix->setRowCount(size);
    ui->matrix->setColumnCount(size);
    ui->matrix->horizontalHeader()->setVisible(false);
    ui->matrix->verticalHeader()->setVisible(false);
    ui->matrix->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->matrix->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->matrix->setEditTriggers(QAbstractItemView::NoEditTriggers);
    auto cov = getCovariance(numLists);
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            //保留两位小数
            float val = round(cov(i, j) * 100) / 100.0;
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(val));
            item->setTextAlignment(Qt::AlignCenter);
            ui->matrix->setItem(i, j, item);
        }
    }
    //创建colorBar
//    QLinearGradient gr;
//    gr.setColorAt(0.0, Qt::red);
//    gr.setColorAt(1.0, Qt::yellow);
    QFrame *colorRect = new QFrame();
    QLinearGradient gradient(0, 0, 0, colorRect->height());
    gradient.setColorAt(0.0, Qt::yellow);
    gradient.setColorAt(0.5, Qt::red);
    gradient.setColorAt(1.0, Qt::green);
    colorRect->setFrameStyle(QFrame::Box);
    colorRect->setFrameShadow(QFrame::Plain);
    colorRect->setStyleSheet(QString("background-color: %1;").arg(gradient.stops().first().second.name()));
    ui->colorLayout->addWidget(colorRect);
}

CovarianceMatrix::~CovarianceMatrix()
{
    delete ui;
}
