#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "csvdata.h"
#include <QMainWindow>
#include <qtableview.h>
#include <QLabel>
#include <qpushbutton.h>
#include <QStandardItemModel>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showTableWindow(CsvData data);
    void initStatusBar();
    void initLayout();
signals:
    void columnSelected(int);
public slots:
    void openFile();
    void handleHeaderClicked(int);
    void getMeanVar();
    void openHistogram();

private:
    Ui::MainWindow *ui;
    CsvData data;
    QVBoxLayout * leftLayout;
    QVBoxLayout * rightLayout;
    QTableView * tableView;
    QLabel * meanValue;
    QLabel * varianceValue;
    QPushButton * updateButton;
    QStandardItemModel * model;
    int cursorColumnIndex;


};
#endif // MAINWINDOW_H
