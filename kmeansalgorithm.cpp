#include "kmeansalgorithm.h"
//Eigen库
#include "Eigen/Dense"
#include "kmeans.hpp"
#include "qdebug.h"
#include <iostream>

KMeansAlgorithm::KMeansAlgorithm(QObject *parent)
    : QObject{parent}
{

}
void KMeansAlgorithm::KMeansAnalysis(CsvData * data, const std::vector<int>& cols, int k)
{
    //从data中筛选出选中的列,组成新的矩阵
    std::vector<std::vector<float>> originMatrix;
    for (const auto & p : data->AllPerson())
    {
        std::vector<float> originDot;
        for (int idx : cols)
        {
            if (idx != 1)
            {
                originDot.emplace_back(p.Data().at(idx - 2));
            }
            else
            {
                originDot.emplace_back(data->MapList()[p.Diagnosis()]);
            }
        }
        originMatrix.emplace_back(originDot);
    }
    auto res = clusterKMeans(originMatrix, k);
    std::vector<int> typeList = std::get<1>(res);
    data->setTypeList(typeList);
    data->setIsClustered(true);
}
