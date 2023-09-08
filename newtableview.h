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
    NewTableView(CsvData data, QWidget * parent = nullptr);
    int selectedCnt();
    void selectedColumnClear();
    QSet<int> SelectedColumns();
public slots:
    void headerClicked(int);

private:
    QSet<int> selectedColumns;
};

#endif // NEWTABLEVIEW_H
