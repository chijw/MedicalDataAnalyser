#ifndef CSVDATA_H
#define CSVDATA_H
#include "person.h"
#include <QStringList>
#include <QString>
#include <QVector>
#include <QFile>
#include <QMap>

typedef QVector<Person> PersonList;

class CsvData
{
public:
    CsvData(QFile * file);
    CsvData();
    bool IsValid() const;
    int rowSize() const;
    int colSize() const;
    QStringList Titles() const;
    PersonList AllPerson() const;
    const QMap<QString,int> MapList() const;
private:
    //存放数据的名称
    QStringList titles;
    //存放个人的各项数据
    PersonList allPerson;
    //字母与整数型数据的映射表
    static const QMap<QString,int> mapList;
    //是否读取到不当数据
    bool isValid;
};

#endif // CSVDATA_H
