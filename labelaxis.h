#ifndef LABELAXIS_H
#define LABELAXIS_H

#include <QWidget>

class LabelAxis : public QWidget
{
    Q_OBJECT
public:
    explicit LabelAxis(float minVal, float maxVal, QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent * event) override;
signals:
private:
    float minValue;
    float maxValue;
};

#endif // LABELAXIS_H
