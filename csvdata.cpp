#include "csvdata.h"
#include "person.h"
#include <QTextStream>
#include <qmessagebox.h>

const QMap<QString, int> CsvData::mapList = {
    { "B" , 0 },
    { "M" , 1 }
};
CsvData::CsvData()
{
}
CsvData::CsvData(QFile * file) : isClustered(false)
{
    isValid = true;
    QTextStream text(file);
    QString titleList = text.readLine();
    this->titles = titleList.split(',');
    while(!file->atEnd())
    {
        QString line = text.readLine();
        Person p(line);
        //更新数据丢失情况
        isValid = p.IsValid();
        allPerson.emplaceBack(p);
    }
    //填充typeList
    for (const auto & p : allPerson)
    {
        if (p.Diagnosis() == "B")
        {
            typeList.emplace_back(0);
        }
        else
        {
            typeList.emplace_back(1);
        }
    }
}
bool CsvData::IsValid() const
{
    return isValid;
}
int CsvData::rowSize() const
{
    return allPerson.size() + 1;
}
int CsvData::colSize() const
{
    return titles.size();
}
QStringList CsvData::Titles() const
{
    return titles;
}
PersonList CsvData::AllPerson() const
{
    return allPerson;
}
const QMap<QString,int> CsvData::MapList() const
{
    return mapList;
}
std::vector<float> CsvData::getColData(int idx) const
{
    std::vector<float> list;
    if (idx > 1)
    {
        for (const auto & p : allPerson)
        {
            list.emplace_back(p.Data().at(idx - 2));
        }
    }
    else if (idx == 1)
    {
        for (const auto & p : allPerson)
        {
            list.emplace_back(MapList()[p.Diagnosis()]);
        }
    }
    return list;
}
void CsvData::setTypeList(std::vector<int> list)
{
    typeList = list;
}
std::vector<int> CsvData::TypeList() const
{
    return typeList;
}
void CsvData::setIsClustered(bool flag)
{
    isClustered = flag;
}
bool CsvData::IsClustered() const
{
    return isClustered;
}
void CsvData::addTitles(QString title)
{
    titles.append(title);
}
