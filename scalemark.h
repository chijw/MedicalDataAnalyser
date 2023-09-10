#ifndef SCALEMARK_H
#define SCALEMARK_H

#include <QWidget>

class ScaleMark : public QWidget
{
    Q_OBJECT
public:
    explicit ScaleMark(float minVal, float maxVal, QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent * event) override;
signals:
private:
    float minValue;
    float maxValue;

};

#endif // SCALEMARK_H
