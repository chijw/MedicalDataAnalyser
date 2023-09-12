#ifndef KMEANSALGORITHM_H
#define KMEANSALGORITHM_H

#include "csvdata.h"
#include <QObject>
#include <vector>

class KMeansAlgorithm : public QObject
{
    Q_OBJECT
public:
    explicit KMeansAlgorithm(QObject *parent = nullptr);
    void KMeansAnalysis(CsvData * data, const std::vector<int>& cols, int k);
signals:

};

#endif // KMEANSALGORITHM_H
