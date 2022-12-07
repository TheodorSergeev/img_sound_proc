#ifndef UTILS
#define UTILS

#include "Eigen/Dense"
#include <opencv2/opencv.hpp>

using Eigen::MatrixXi;
using Eigen::MatrixXd;

MatrixXi opencv2eigen(const cv::Mat& image);
cv::Mat eigen2opencv(const MatrixXi& mat);
#endif