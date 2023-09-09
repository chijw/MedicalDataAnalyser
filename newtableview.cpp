#include "newtableview.h"

NewTableView::NewTableView(CsvData data, QWidget * parent) :
    QTableView(parent)
{
    model = new QStandardItemModel;
    model->setHorizontalHeaderLabels(data.Titles());
    initFont = model->horizontalHeaderItem(0)->font();
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
    if (idx > 0)
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
                //setStyleSheet(QString("QHeaderView::section:checked:horizontal{background:red;}"));
        }
        else
        {
            selectedColumns.remove(idx);
            // 恢复选中列的字体样式
            model->horizontalHeaderItem(idx)->setFont(initFont);
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
