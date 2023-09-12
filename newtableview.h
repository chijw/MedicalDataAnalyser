#ifndef NEWTABLEVIEW_H
#define NEWTABLEVIEW_H

#include "csvdata.h"
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QSet>

class NewTableView : public QTableView
{
    Q_OBJECT
public:
    NewTableView(CsvData * input, QWidget * parent = nullptr);
    NewTableView(QWidget * parent = nullptr);
    int selectedCnt();
    void selectedColumnClear();
    QSet<int> SelectedColumns();
    static const QColor colorList[];
public slots:
    void headerClicked(int);
    void showCluster(bool state);
private:
    QSet<int> selectedColumns;
    QStandardItemModel * model;
    QFont initFont;
    CsvData * data;

};

#endif // NEWTABLEVIEW_H
