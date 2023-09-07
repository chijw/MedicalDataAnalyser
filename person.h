#ifndef PERSON_H
#define PERSON_H
#include <QString>
#include <QVector>

typedef QVector<float> DataList;

class Person
{
public:
    Person(QString list);
    bool IsValid() const;
    QString Id() const;
    QString Diagnosis() const;
    DataList Data() const;
private:
    QString id;
    QString diagnosis;
    //以double形式存储的数据
    DataList data;
    //是否读取到不当数据
    bool isValid;
};

#endif // PERSON_H
