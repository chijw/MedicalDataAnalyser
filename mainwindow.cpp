#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "histogram.h"
#include "scattergram.h"
#include "curvegraph.h"
#include "covariancematrix.h"
#include "csvdata.h"
#include "pca2dgraph.h"
#include "pca3dgraph.h"
#include "kmeansalgorithm.h"
//Eigen库
#include "Eigen/Dense"
#include "rowfeature.hpp"
#include <vector>
#include <iostream>

#include <qfiledialog.h>
#include <QFile>
#include <qmessagebox.h>
#include <qtablewidget.h>
#include <qtableview.h>
#include <QStandardItem>
#include <QVBoxLayout>
#include <QDebug>
#include <QStatusBar>
#include <QLabel>
#include <QHeaderView>
#include <QToolTip>
#include <QInputDialog>

/*
 * 编辑状态栏
 * 添加数据均值、方差的显示栏和数据更新按钮
*/
void MainWindow::initStatusBar()
{
    QStatusBar * stBar = new QStatusBar;
    setStatusBar(stBar);

    updateButton = new QPushButton;
    updateButton->setText("更新");
    updateButton->setEnabled(false);
    stBar->addPermanentWidget(updateButton);

    QLabel * mean = new QLabel("均值:",this);
    stBar->addPermanentWidget(mean);
    meanValue = new QLabel("0",this);
    stBar->addPermanentWidget(meanValue);

    QLabel * variance = new QLabel("方差:",this);
    stBar->addPermanentWidget(variance);
    varianceValue = new QLabel("0",this);
    stBar->addPermanentWidget(varianceValue);

    clusterButton = new QPushButton;
    clusterButton->setText("显示分类");
    stBar->addWidget(clusterButton);

    clearButton = new QPushButton;
    clearButton->setText("取消选中");
    stBar->addWidget(clearButton);
}
/*
 * 主窗口布局创建
*/
void MainWindow::initLayout()
{
    QWidget * leftWidget = new QWidget;
    QWidget * rightWidget = new QWidget;
    leftLayout = new QVBoxLayout(leftWidget);
    rightLayout = new QVBoxLayout(rightWidget);
    QSplitter * splitter = new QSplitter;
    splitter->addWidget(leftWidget);
    splitter->addWidget(rightWidget);
    setCentralWidget(splitter);
    //设置右侧部件的splitter
    bySplitter = new QSplitter(Qt::Vertical);
    rightLayout->addWidget(bySplitter);
}
/*
 * 用读取的表格数据加载表格界面
*/
void MainWindow::showTableWindow()
{
    if (tableView != nullptr)
    {
        delete tableView;
    }
    //创建新的tableView
    tableView = new NewTableView(&data);
    //布局控制
    leftLayout->addWidget(tableView);
    //完成相关信号的连接
    connect(this, &MainWindow::cluterBtnClicked, tableView, &NewTableView::showCluster);
    connect(tableView->horizontalHeader(), &QHeaderView::sectionPressed, this, &MainWindow::handleHeaderClicked);
    connect(this, &MainWindow::columnSelected, this, [=](int col){
        cursorColumnIndex = col;
    });
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::getMeanVar);
    //连接kmeans聚类完成信号和增加列事件
    connect(this, &MainWindow::doneKMeansClusterAnalysis, tableView, &NewTableView::addColumns);
    //为取消选中列绑定函数
    connect(clearButton, &QPushButton::clicked, tableView, &NewTableView::clearSelectedColumns);
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), cursorColumnIndex(-1), cluterBtnState(false)
    , ui(new Ui::MainWindow)
{
    //初始化窗口
    ui->setupUi(this);
    resize(1200,800);
    initLayout();
    initStatusBar();
    //创建tableView
    tableView = new NewTableView;
    //为显示分类绑定函数
    connect(clusterButton, &QPushButton::clicked, this, &MainWindow::handleCluterClicked);
    //为菜单栏的各种操作绑定函数
    connect(ui->actionimport, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionDrawHistogram, &QAction::triggered, this, &MainWindow::openHistogram);
    connect(ui->actionDrawScattergram, &QAction::triggered, this, &MainWindow::openScattergram);
    connect(ui->actionDrawCurve, &QAction::triggered, this, &MainWindow::drawCurveGraph);
    connect(ui->actionDrawCovarianceMatrix, &QAction::triggered, this, &MainWindow::drawCovarianceMatrix);
    connect(ui->action2D, &QAction::triggered, this, &MainWindow::do2DPCA);
    connect(ui->action3D, &QAction::triggered, this, &MainWindow::do3DPCA);
    connect(ui->actionKMeans, &QAction::triggered, this, &MainWindow::KMeansCluster);
    connect(this,&MainWindow::wrongOption,this,&MainWindow::wrongOptionHint);
}
MainWindow::~MainWindow()
{
    delete ui;
}
/*
 * 打开文件
*/
void MainWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(this,"打开文件","D:/Desktop","*.csv");
    QFile file(path);
    if (!file.exists())
    {
        QMessageBox::warning(this,"警告","文件不存在！");
        return;
    }
    else if (file.size() == 0)
    {
        QMessageBox::warning(this,"警告","文件为空");
        return;
    }

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        data = CsvData(&file);
        showTableWindow();
        if (!data.IsValid())
        {
            QMessageBox::information(this,"提示","部分数据丢失或格式有误");
        }
    }
    else
    {
        QMessageBox::warning(this,"警告","无法打开文件");
    }
}
void MainWindow::handleHeaderClicked(int col)
{
    //若不为id列，发射列被选中信号，启用数据更新按钮
    if(col >= 1)
    {
        updateButton->setEnabled(true);
    }
    //否则，禁用数据更新按钮
    else
    {
        updateButton->setEnabled(false);
    }
    emit columnSelected(col);
}
/*
 * 获取平均值和方差
*/
void MainWindow::getMeanVar()
{
    std::vector<float>row;
    if (cursorColumnIndex > 1 && cursorColumnIndex < tableView->Model()->columnCount() - data.IsClustered())
    {
        for (const auto & p : data.AllPerson())
        {
            row.emplace_back(p.Data().at(cursorColumnIndex - 2));
        }
    }
    else if (cursorColumnIndex == 1)
    {
        for (const auto & p : data.AllPerson())
        {
            row.emplace_back(data.MapList()[p.Diagnosis()]);
        }
    }
    else
    {
        return;
    }
    std::tuple<float, float> AvgVar = getAvgVar(row);
    meanValue->setText(QString::number(std::get<0>(AvgVar)));
    varianceValue->setText(QString::number(std::get<1>(AvgVar)));
}
/*
 * 处理分类显示按钮
*/
void MainWindow::handleCluterClicked()
{
    if (!cluterBtnState)
    {
        clusterButton->setText("取消显示");
    }
    else
    {
        clusterButton->setText("显示分类");
    }
    emit cluterBtnClicked(cluterBtnState);
    cluterBtnState = !cluterBtnState;
}
/*
 * 打开直方图
*/
void MainWindow::openHistogram()
{
    if(cursorColumnIndex >= 1 && cursorColumnIndex < tableView->Model()->columnCount() - data.IsClustered())
    {
        Histogram * histogram = new Histogram(cursorColumnIndex, data);
        bySplitter->addWidget(histogram);
    }
}
void MainWindow::openScattergram()
{
    if (tableView->selectedCnt() == 2)
    {
        std::vector<std::vector<float>>numLists;
        QStringList titles;
        foreach (int ele, tableView->SelectedColumns())
        {
            numLists.emplace_back(data.getColData(ele));
            titles.emplaceBack(data.Titles().at(ele));
        }
        Scattergram * scattergram = new Scattergram(numLists[0], numLists[1], titles);
        bySplitter->addWidget(scattergram);
    }
    else
    {
        emit wrongOption();
    }

}
void MainWindow::drawCurveGraph()
{
    if (tableView->selectedCnt() == 2)
    {
        std::vector<std::vector<float>>numLists;
        QStringList titles;
        foreach (int ele, tableView->SelectedColumns())
        {
            numLists.emplace_back(data.getColData(ele));
            titles.emplaceBack(data.Titles().at(ele));
        }
        CurveGraph * curveGraph = new CurveGraph(numLists[0], numLists[1], titles);
        curveGraph->show();
    }
    else
    {
        emit wrongOption();
    }
}
void MainWindow::drawCovarianceMatrix()
{
    if (tableView->selectedCnt() > 1)
    {
        std::vector<std::vector<float>>numLists;
        QStringList titles;
        foreach (int ele, tableView->SelectedColumns())
        {
            numLists.emplace_back(data.getColData(ele));
            titles.emplaceBack(data.Titles().at(ele));
        }
        CovarianceMatrix * covaMatrix = new CovarianceMatrix(numLists,titles);
        covaMatrix->show();
    }
    else
    {
        emit wrongOption();
    }
}
void MainWindow::do2DPCA()
{
    if (tableView->selectedCnt() > 1)
    {
        std::vector<int> cols;
        foreach (int ele, tableView->SelectedColumns())
        {
            cols.push_back(ele);
        }
        PCA2DGraph * graph_2D = new PCA2DGraph(data, cols);
        graph_2D->show();
    }
    else
    {
        emit wrongOption();
    }
}
void MainWindow::do3DPCA()
{
    if (tableView->selectedCnt() > 2)
    {
        std::vector<int> cols;
        foreach (int ele, tableView->SelectedColumns())
        {
            cols.push_back(ele);
        }
        PCA3DGraph * graph_3D = new PCA3DGraph(data, cols);
        graph_3D->show();
    }
    else
    {
        emit wrongOption();
    }
}
void MainWindow::KMeansCluster()
{
    if (tableView->selectedCnt() > 0)
    {
        int k = QInputDialog::getInt(nullptr,"设置","设置类数（1-10）：",5,1,10);
        std::vector<int> cols;
        foreach (int ele, tableView->SelectedColumns())
        {
            cols.push_back(ele);
        }
        KMeansAlgorithm ka;
        //将kmeans算法完成信号传递到主窗口
        connect(&ka, &KMeansAlgorithm::doneKMeansClusterAnalysis, this, &MainWindow::doneKMeansClusterAnalysis);
        ka.KMeansAnalysis(&data, cols, k);
    }
    else
    {
        emit wrongOption();
    }
}
void MainWindow::wrongOptionHint()
{
    int x = geometry().center().x() - 70;
    int y = geometry().top() + 10;
    QToolTip::showText(QPoint(x,y),"请选择正确数量的列");
}
