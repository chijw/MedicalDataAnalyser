#include "covariancematrix.h"
#include "ui_covariancematrix.h"

#include <QLabel>
#include <QSpacerItem>
#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QLayout>
static const int tableX = 50;
static const int tableY = 10;
static const int tableWidth = 560;
static const int tableHeight = 560;
CovarianceMatrix::CovarianceMatrix(const std::vector<std::vector<float>>& numLists,
                   const QStringList& titles, QWidget *parent) :
    QDialog(parent), state(true), ui(new Ui::CovarianceMatrix)
{
    ui->setupUi(this);
    size = numLists.size();
    //设置表格的标签
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
    //设置竖直表格标签
    QGraphicsScene *scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    //设置graphics无边框、背景透明
    ui->graphicsView->setStyleSheet("background: transparent; border : none");
    float sceneHeight = ui->graphicsView->height();
    float space = sceneHeight / size;
    for (int i = 0; i < size; ++i)
    {
        QGraphicsTextItem *text = scene->addText(titles[size - i - 1]);
        text->setRotation(-90);
        // 获取文本的高度
        float textHeight = text->boundingRect().height();
        // 计算文本的位置，使其居中显示在场景上
        float x = 0;
        float y = (i + 1) * space - textHeight / 2;
        // 设置文本的位置
        text->setPos(x, y);
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
    cov = getCovariance(numLists);
    std::vector<float> variance;
    for (const auto & list : numLists)
    {
        auto res = getAvgVar(list);
        variance.emplace_back(std::get<1>(res));
    }
    cor = getPearsonCorr(cov, variance);
    covMinValue = round(cov(0,0) * 100) / 100.0;
    covMaxValue = round(cov(0,0) * 100) / 100.0;
    corMinValue = round(cor(0,0) * 100) / 100.0;
    corMaxValue = round(cor(0,0) * 100) / 100.0;
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            //保留两位小数
            float covVal = round(cov(i, j) * 100) / 100.0;
            float corVal = round(cor(i, j) * 100) / 100.0;
            //更新最大、最小值
            covMinValue = std::min(covMinValue, covVal);
            covMaxValue = std::max(covMaxValue, covVal);
            corMinValue = std::min(corMinValue, corVal);
            corMaxValue = std::max(corMaxValue, corVal);
        }
    }
    covColorBar = new ColorBar(covMinValue, covMaxValue);
    //为cor矩阵创建一个colorBar，作用是使用其getColor函数获取需要插入的颜色的值
    corColorBar = new ColorBar(corMinValue, corMaxValue);
    ui->colorLayout->addWidget(covColorBar);
    scaleMark = new ScaleMark(covMinValue, covMaxValue);
    ui->scaleMarkLayout->addWidget(scaleMark);
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            //保留两位小数
            float covVal = round(cov(i, j) * 100) / 100.0;
            QTableWidgetItem * covItem = new QTableWidgetItem(QString::number(covVal));
            if (covVal > (covMaxValue + covMinValue) / 2)
            {
                covItem->setForeground(Qt::white);
            }
            QBrush covBrush(covColorBar->getColor(covVal));
            covItem->setBackground(covBrush);
            covItem->setTextAlignment(Qt::AlignCenter);
            ui->matrix->setItem(i, j, covItem);
        }
    }
    connect(ui->switchBtn, &QPushButton::clicked, this, &CovarianceMatrix::changeState);
}
CovarianceMatrix::~CovarianceMatrix()
{
    delete ui;
}
void CovarianceMatrix::changeState()
{
    if (state)
    {
        state = false;
        ui->switchBtn->setText("协方差矩阵");
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                //保留两位小数
                float corVal = round(cor(i, j) * 100) / 100.0;
                //回收原来的item
                QTableWidgetItem * originalItem = ui->matrix->takeItem(i, j);
                delete originalItem;
                originalItem = nullptr;
                QTableWidgetItem * corItem = new QTableWidgetItem(QString::number(corVal));
                if (corVal > (corMaxValue + corMinValue) / 2)
                {
                    corItem->setForeground(Qt::white);
                }
                QBrush corBrush(corColorBar->getColor(corVal));
                corItem->setBackground(corBrush);
                corItem->setTextAlignment(Qt::AlignCenter);
                ui->matrix->setItem(i, j, corItem);
            }
        }
        //重绘scaleMark
        delete scaleMark;
        scaleMark = new ScaleMark(corMinValue, corMaxValue);
        ui->scaleMarkLayout->addWidget(scaleMark);
    }
    else
    {
        state = true;
        ui->switchBtn->setText("相关系数矩阵");
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                //保留两位小数
                float covVal = round(cov(i, j) * 100) / 100.0;
                QTableWidgetItem * originalItem = ui->matrix->takeItem(i, j);
                delete originalItem;
                originalItem = nullptr;
                QTableWidgetItem * covItem = new QTableWidgetItem(QString::number(covVal));
                if (covVal > (covMaxValue + covMinValue) / 2)
                {
                    covItem->setForeground(Qt::white);
                }
                QBrush covBrush(covColorBar->getColor(covVal));
                covItem->setBackground(covBrush);
                covItem->setTextAlignment(Qt::AlignCenter);
                ui->matrix->setItem(i, j, covItem);
            }
        }
        delete scaleMark;
        scaleMark = new ScaleMark(covMinValue, covMaxValue);
        ui->scaleMarkLayout->addWidget(scaleMark);
    }
}
