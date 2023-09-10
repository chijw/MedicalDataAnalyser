#ifndef COLORBAR_H
#define COLORBAR_H

#include <QWidget>
#include <QPaintEvent>
#include <QColor>

class ColorBar : public QWidget
{
    Q_OBJECT
public:
    explicit ColorBar(float minValue, float maxValue, QWidget *parent = nullptr);
    //根据输入来返回相应的颜色
    QColor getColor(float val);
protected:
    void paintEvent(QPaintEvent * event) override;
signals:

private:
    QColor colorTop;
    QColor colorMid;
    QColor colorBtn;
    float minValue;
    float maxValue;
};

#endif // COLORBAR_H
