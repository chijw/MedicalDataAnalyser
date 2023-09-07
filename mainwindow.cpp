#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "histogram.h"
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
#include <QScrollArea>

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
/*
 * 用读取的表格数据加载表格界面
*/
void MainWindow::showTableWindow(CsvData data)
{
    //设置model
    model = new QStandardItemModel;
    model->setHorizontalHeaderLabels(data.Titles());
    for (auto & p : data.AllPerson())
    {
        QVector<QStandardItem *> itemRow;
        QStandardItem * idItem = new QStandardItem(p.Id());
        itemRow.emplaceBack(idItem);
        QStandardItem * diagItem = new QStandardItem(p.Diagnosis());
        itemRow.emplaceBack(diagItem);
        for (auto & ele : p.Data())
        {
            QStandardItem * it = new QStandardItem(QString::number(ele));
            itemRow.emplaceBack(it);
        }
        model->appendRow(itemRow);
    }
    //设置tabelView
    tableView = new QTableView;
    tableView->setModel(model);
    QItemSelectionModel * selectionModel = new QItemSelectionModel(model);
    tableView->setSelectionModel(selectionModel);
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
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //初始化窗口
    ui->setupUi(this);
    resize(1200,800);
    //默认将列指针置为 -1
    cursorColumnIndex = -1;
    initLayout();
    initStatusBar();
    //打开文件
    connect(ui->actionimport,&QAction::triggered,this,&MainWindow::openFile);
    connect(ui->actionDrawHistogram,&QAction::triggered,this,&MainWindow::openHistogram);
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
        showTableWindow(data);
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
