#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "csvdata.h"
#include <QDialog>
#include <QLayout>

namespace Ui {
class Histogram;
}

class Histogram : public QDialog
{
    Q_OBJECT

public:
    explicit Histogram(int cursorColumnIndex, CsvData data, QLayout * parentLayout, QWidget *parent = nullptr);
    ~Histogram();

private:
    Ui::Histogram *ui;
};

#endif // HISTOGRAM_H
