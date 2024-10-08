#include "newtableview.h"
#include <QColor>

const QColor NewTableView::colorList[] = {
    QColor(255, 0, 0), QColor(0, 255, 0),
    QColor(0, 0, 255), QColor(255, 255, 0),
    QColor(255, 165, 0), QColor(128, 0, 128),
    QColor(0, 128, 128), QColor(139, 69, 19),
    QColor(255, 192, 203), QColor(111, 111, 211)
};

NewTableView::NewTableView(QWidget * parent) :
QTableView(parent)
{
}
NewTableView::NewTableView(CsvData * input, QWidget * parent) :
    QTableView(parent)
{
    data = input;
    model = new QStandardItemModel;
    model->setHorizontalHeaderLabels(data->Titles());
    initFont = model->horizontalHeaderItem(0)->font();
    for (auto & p : data->AllPerson())
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
    setModel(model);
    QItemSelectionModel * selectionModel = new QItemSelectionModel(model);
    setSelectionModel(selectionModel);
    selectedColumns = {};
    //信号连接
    connect(horizontalHeader(), &QHeaderView::sectionClicked, this, &NewTableView::headerClicked);
}
void NewTableView::headerClicked(int idx)
{
    if (idx > 0 && idx < model->columnCount() - data->IsClustered())
    {
        if (!selectedColumns.contains(idx))
        {
            //加入到选中栏目集合
            selectedColumns.insert(idx);
            // 更改选中列的字体样式
            QFont font = initFont;
            font.setItalic(true);
            font.setBold(true);
            model->horizontalHeaderItem(idx)->setFont(font);
        }
        else
        {
            selectedColumns.remove(idx);
            // 恢复选中列的字体样式
            model->horizontalHeaderItem(idx)->setFont(initFont);
        }
    }
}
void NewTableView::showCluster(bool state)
{

    if (!state)
    {
        //设置表格背景色
        for (int row = 0; row < model->rowCount(); ++row)
        {
            QBrush bkColor(colorList[data->TypeList().at(row)]);
            for (int col = 0; col < model->columnCount(); ++col)
            {
                QModelIndex index = model->index(row, col);
                model->setData(index, bkColor, Qt::BackgroundRole);
            }
        }
    }
    else
    {
        QBrush bkColor = QColor(255,255,255);
        for (int row = 0; row < model->rowCount(); ++row)
        {
            for (int col = 0; col < model->columnCount(); ++col)
            {
                QModelIndex index = model->index(row, col);
                model->setData(index, bkColor, Qt::BackgroundRole);
            }
        }
    }
}
int NewTableView::selectedCnt()
{
    return selectedColumns.size();
}
void NewTableView::selectedColumnClear()
{
    selectedColumns.clear();
}
QSet<int> NewTableView::SelectedColumns()
{
    return selectedColumns;
}
void NewTableView::addColumns(QString tittle)
{
    if (data->IsClustered())
    {
        int colCnt = model->columnCount();
        for (int row = 0; row < model->rowCount(); ++row)
        {
            model->removeColumns(colCnt - 1, row);
        }
    }
    QVector<QStandardItem *> itemColumn;
    for (int i = 0; i < model->rowCount(); ++i)
    {
        QStandardItem * item = new QStandardItem(QString::number(data->TypeList().at(i)));
        item->setTextAlignment(Qt::AlignCenter);
        itemColumn.emplaceBack(item);
    }
    model->appendColumn(itemColumn);
    QStandardItem * headerItem = new QStandardItem(tittle);
    headerItem->setTextAlignment(Qt::AlignCenter);
    model->setHorizontalHeaderItem(model->columnCount() - 1, headerItem);
}
/*
 * 清空所有被选中的列
*/
void NewTableView::clearSelectedColumns()
{
    foreach (int idx, selectedColumns)
    {
        model->horizontalHeaderItem(idx)->setFont(initFont);
    }
    selectedColumns.clear();
}
QStandardItemModel * NewTableView::Model() const
{
    return model;
}
