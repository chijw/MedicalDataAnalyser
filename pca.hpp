#ifndef PCA_HPP
#define PCA_HPP

#include "common.h"

Eigen::MatrixXf pca(const std::vector<std::vector<float>> &in, const int k);

#endif // PCA_HPP
