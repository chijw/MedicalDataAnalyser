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
CsvData::CsvData(QFile * file)
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
