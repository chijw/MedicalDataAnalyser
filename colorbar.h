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
