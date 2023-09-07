#include "newtableview.h"
#include <QPalette>

NewTableView::NewTableView(CsvData data, QWidget * parent) :
    QTableView(parent)
{
    QStandardItemModel * model = new QStandardItemModel;
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
    this->setModel(model);
    QItemSelectionModel * selectionModel = new QItemSelectionModel(model);
    this->setSelectionModel(selectionModel);
    selectedColumns.clear();
    //信号连接
    connect(horizontalHeader(), &QHeaderView::sectionClicked, this, &NewTableView::headerClicked);

}
void NewTableView::headerClicked(int idx)
{
    if (selectedColumns.contains(idx))
    {
        selectedColumns.remove(idx);
        // 恢复表头背景颜色
        QPalette palette = horizontalHeader()->palette();
        palette.setColor(QPalette::Window, Qt::transparent);
        horizontalHeader()->setPalette(palette);
    }
    else
    {
        selectedColumns.insert(idx);
        // 更改选中列的背景颜色
        QPalette palette = horizontalHeader()->palette();
        palette.setColor(QPalette::Window, Qt::blue);
        horizontalHeader()->setPalette(palette);
    }
}
