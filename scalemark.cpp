#include "scalemark.h"
#include <QPainter>

ScaleMark::ScaleMark(float minVal, float maxVal, QWidget *parent)
    : QWidget{parent}, minValue(minVal), maxValue(maxVal)
{

}
void ScaleMark::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    int tickCnt = 5;
    float portion = (maxValue - minValue) / tickCnt;
    for (int i = 1; i <= tickCnt; ++i)
    {
        //保留两位小数
        float val = round((minValue + portion * i) * 100) / 100.0;
        painter.drawText(0 ,10 + height() - i * height() / tickCnt, QString("-%1").arg(val));
    }
}

