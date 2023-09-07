#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "csvdata.h"
#include <QDialog>
#include <QLayout>

namespace Ui {
class Histogram;
}

class Histogram : public QWidget
{
    Q_OBJECT

public:
    explicit Histogram(int cursorColumnIndex, CsvData data,  QWidget *parent = nullptr);
    ~Histogram();

};

#endif // HISTOGRAM_H

