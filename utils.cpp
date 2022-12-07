#include "utils.hpp"

MatrixXi opencv2eigen(const cv::Mat& image) {
    int n_cols = image.cols;
    int n_rows = image.rows;
    MatrixXi mat(n_cols, n_rows);

    for (int i = 0; i < n_cols; ++i) {
        for (int j = 0; j < n_rows; ++j) {
            mat(i, j) = (int) image.at<char>(i, j);
        }
    }

    return mat;
}

cv::Mat eigen2opencv(const MatrixXi& mat) {
    int n_cols = mat.cols();
    int n_rows = mat.rows();
    cv::Mat image = cv::Mat::zeros(n_cols, n_rows, CV_8U);

    for (int i = 0; i < n_cols; ++i) {
        for (int j = 0; j < n_rows; ++j) {
            image.at<char>(i, j) = mat(i, j);
        }
    }

    return image;
}
