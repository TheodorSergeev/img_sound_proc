#ifndef UTILS
#define UTILS

#include <iostream>
#include <string>
#include <filesystem>
#include "Eigen/Dense"
#include "AudioFile.h"
#include <opencv2/opencv.hpp>

using std::cout;
using std::string;
using Eigen::MatrixXi;
using Eigen::MatrixXd;

/**
 * @brief Read integer matrix from a file.
 * Supported formats: image (e.g. png and tiff).
 * 
 * @param inp_fname Input filename. 
 * @return MatrixXi Read matrix.
 */
MatrixXi readIntMatrix(const string& inp_fname);

/**
 * @brief Save integer matrix in a file.
 * Supported formats: image (e.g. png and tiff).
 * 
 * @param out_fname 
 * @param matrix 
 */
void writeIntMatrix(const string& out_fname, const MatrixXi& matrix);
MatrixXd readFloatMatrix(const string& inp_fname);

/**
 * @brief Convert opencv matrix to eigen integer matrix.
 * 
 * @param image Matrix in the opencv format.
 * @return MatrixXi Integer matrix in the eigen format.
 */
MatrixXi opencv2eigen(const cv::Mat& image);

/**
 * @brief Convert eigen matrix to opencv matrix.
 * 
 * @param mat Integer matrix in the eigen format.
 * @return cv::Mat Matrix in the opencv format.
 */
cv::Mat eigen2opencv(const MatrixXi& mat);
#endif