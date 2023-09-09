#include "colorbar.h"
#include <QPainter>
#include <QLinearGradient>

ColorBar::ColorBar(float minVal, float maxVal, QWidget *parent)
    : QWidget{parent}, minValue(minVal), maxValue(maxVal)
{
    colorTop.setHsv(0,255,150);
    colorBtn.setHsv(0,0,255);
}
void ColorBar::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    QLinearGradient gradient(rect().topLeft(), rect().bottomLeft());
    gradient.setColorAt(0.0,colorTop);
    gradient.setColorAt(1.0,colorBtn);
    painter.fillRect(rect(), gradient);
}
