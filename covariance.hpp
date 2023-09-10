#ifndef COVARIANCE_HPP
#define COVARIANCE_HPP

#include "common.h"
#include "rowfeature.hpp"

Eigen::MatrixXf getCovariance(const std::vector<std::vector<float>> &inMat);
Eigen::MatrixXf getPearsonCorr(const Eigen::MatrixXf &cov, const std::vector<float> &vars);

#endif // COVARIANCE_HPP
