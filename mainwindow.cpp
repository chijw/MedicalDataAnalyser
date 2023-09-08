#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "histogram.h"
#include "scattergram.h"
#include "csvdata.h"
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

/*
 * 编辑状态栏
 * 添加数据均值、方差的显示栏和数据更新按钮
*/
void MainWindow::initStatusBar()
{
    QStatusBar * stBar = new QStatusBar();
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
/* TODO
 * TODO 用读取的表格数据加载表格界面
*/
void MainWindow::showTableWindow()
{
    //一次只允许存在一个tableView
    delete tableView;
    tableView = nullptr;
    //创建新的tableView
    tableView = new NewTableView(data);
    //布局控制
    leftLayout->addWidget(tableView);
    //完成相关信号的连接
    connect(tableView->horizontalHeader(), &QHeaderView::sectionPressed, this, &MainWindow::handleHeaderClicked);
    connect(this, &MainWindow::columnSelected, this, [=](int col){
        cursorColumnIndex = col;
    });
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::getMeanVar);


}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),cursorColumnIndex(-1)
    , ui(new Ui::MainWindow)
{
    //初始化窗口
    ui->setupUi(this);
    resize(1200,800);
    initLayout();
    initStatusBar();
    //打开文件
    connect(ui->actionimport,&QAction::triggered,this,&MainWindow::openFile);
    connect(ui->actionDrawHistogram,&QAction::triggered,this,&MainWindow::openHistogram);
    connect(ui->actionDrawScattergram,&QAction::triggered,this,&MainWindow::openScattergram);
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
    if (cursorColumnIndex != 1)
    {
        for (const auto & p : data.AllPerson())
        {
            row.emplace_back(p.Data().at(cursorColumnIndex - 2));
        }
    }
    else
    {
        for (const auto & p : data.AllPerson())
        {
            row.emplace_back(data.MapList()[p.Diagnosis()]);
        }
    }
    std::tuple<float, float> AvgVar = getAvgVar(row);
    meanValue->setText(QString::number(std::get<0>(AvgVar)));
    varianceValue->setText(QString::number(std::get<1>(AvgVar)));
}
/*
 * 打开直方图
*/
void MainWindow::openHistogram()
{
    Histogram * histogram = new Histogram(cursorColumnIndex, data);
    if(cursorColumnIndex >= 1)
    {
        bySplitter->addWidget(histogram);
    }
}
void MainWindow::openScattergram()
{
    if (tableView->selectedCnt() == 2)
    {
        std::vector<int> allIdx;
        foreach (int ele, tableView->SelectedColumns())
        {
            allIdx.emplace_back(ele);
        }
        Scattergram * scattergram = new Scattergram(allIdx[0], allIdx[1], data);
        bySplitter->addWidget(scattergram);
        tableView->selectedColumnClear();
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
