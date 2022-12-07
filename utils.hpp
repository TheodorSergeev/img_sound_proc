#ifndef UTILS
#define UTILS

#include "Eigen/Dense"
#include <opencv2/opencv.hpp>

using Eigen::MatrixXi;
using Eigen::MatrixXd;

MatrixXi readIntMatrix(const string& inp_fname);
void writeIntMatrix(const string& out_fname, const MatrixXi& matrix);
MatrixXd readFloatMatrix(const string& inp_fname);

MatrixXi opencv2eigen(const cv::Mat& image);
cv::Mat eigen2opencv(const MatrixXi& mat);
#endif