#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "csvdata.h"
#include "newtableview.h"
#include <QMainWindow>
#include <qtableview.h>
#include <QLabel>
#include <qpushbutton.h>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QSplitter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showTableWindow();
    void initStatusBar();
    void initLayout();
signals:
    void columnSelected(int);
    void wrongOption();
public slots:
    void openFile();
    void handleHeaderClicked(int);
    void wrongOptionHint();
    void getMeanVar();
    void openHistogram();
    void openScattergram();
    void drawCurveGraph();
private:
    Ui::MainWindow *ui;
    CsvData data;
    QVBoxLayout * leftLayout;
    QVBoxLayout * rightLayout;   
    //用于创建tableview
    NewTableView * tableView;
    QStandardItemModel * model;
    QLabel * meanValue;
    QLabel * varianceValue;
    QPushButton * updateButton;
    int cursorColumnIndex;
    QSplitter * bySplitter;
};
#endif // MAINWINDOW_H
