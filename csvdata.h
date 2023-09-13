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
    std::vector<float> getColData(int idx) const;
    void setTypeList(std::vector<int> list);
    std::vector<int> TypeList() const;
    void setIsClustered(bool flag);
    bool IsClustered() const;
    void addTitles(QString title);
private:
    //存放数据的名称
    QStringList titles;
    //存放个人的各项数据
    PersonList allPerson;
    //字母与整数型数据的映射表
    static const QMap<QString,int> mapList;
    //是否读取到不当数据
    bool isValid;
    std::vector<int> typeList;
    //记录是否被聚类分析过
    bool isClustered;
};

#endif // CSVDATA_H
