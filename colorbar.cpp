#include "colorbar.h"
#include <QPainter>
#include <QLinearGradient>

ColorBar::ColorBar(float minVal, float maxVal, QWidget *parent)
    : QWidget{parent}, minValue(minVal), maxValue(maxVal)
{
    colorTop.setHsv(0,255,150);
    colorMid.setHsv(50,200,255);
    colorBtn.setHsv(0,0,255);
}
void ColorBar::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    QLinearGradient gradient(rect().topLeft(), rect().bottomLeft());
    gradient.setColorAt(0.0,colorTop);
    gradient.setColorAt(0.5,colorMid);
    gradient.setColorAt(1.0,colorBtn);
    painter.fillRect(rect(), gradient);
}
QColor ColorBar::getColor(float val)
{
    //根据val的值判断其对应的颜色
    float midValue = (maxValue + minValue) / 2;
    QColor tempColor;
    float portion;
    int H, S, V;
    if (val > midValue)
    {
        portion = (val - midValue) / (maxValue - midValue);
        H = static_cast<int>(portion * (-50) + 50);
        S = static_cast<int>(portion * 55 + 200);
        V = static_cast<int>(portion * (-105) + 255);
        tempColor.setHsv(H,S,V);
    }
    else
    {
        portion = (val - minValue) / (midValue - minValue);
        H = static_cast<int>(portion * 50);
        S = static_cast<int>(portion * 200);
        V = static_cast<int>(255);
        tempColor.setHsv(H,S,V);
    }
    return tempColor;
}
