#include "person.h"

Person::Person(QString list)
{
    isValid = true;
    QStringList field = list.split(",");
    id = field[0];
    diagnosis = field[1];
    for (int i = 2; i < field.size(); ++i)
    {
        //将字符串转化为float类型
        bool ok;
        float value = field[i].toFloat(&ok);
        if (ok)
        {
            data.emplaceBack(value);
        }
        else
        {
            //如果转化时出现问题，将该person的数据标记为有问题的数据并在对应位置填入0.0
            data.emplaceBack(0.0);
            isValid = false;
        }
    }
    if (diagnosis == "B")
    {
        type = 0;
    }
    else
    {
        type = 1;
    }
}
bool Person::IsValid() const
{
    return isValid;
}
QString Person::Id() const
{
    return id;
}
QString Person::Diagnosis() const
{
    return diagnosis;
}
DataList Person::Data() const
{
    return data;
}
void Person::setType(int tp)
{
    type = tp;
}
