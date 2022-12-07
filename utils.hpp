#ifndef UTILS
#define UTILS

#include <iostream>
#include <string>
#include "Eigen/Dense"
#include "AudioFile.h"
#include <opencv2/opencv.hpp>
#include <filesystem>

using std::cout;
using std::string;
using Eigen::MatrixXi;
using Eigen::MatrixXd;

MatrixXi readIntMatrix(const string& inp_fname);
void writeIntMatrix(const string& out_fname, const MatrixXi& matrix);
MatrixXd readFloatMatrix(const string& inp_fname);

MatrixXi opencv2eigen(const cv::Mat& image);
cv::Mat eigen2opencv(const MatrixXi& mat);
#endif