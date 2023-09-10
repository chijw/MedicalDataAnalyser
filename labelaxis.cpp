#include "labelaxis.h"
#include <QPainter>

LabelAxis::LabelAxis(float minVal, float maxVal, QWidget *parent)
    : QWidget{parent}, minValue(minVal), maxValue(maxVal)
{

}
void LabelAxis::paintEvent(QPaintEvent * event)
{

}
