#include "transforms.hpp"


// Transform

// doesn't work for some reason
//template <typename TInput, typename TOutput>
//TOutput Transform<TInput, TOutput>::transform(const TInput& item_) = 0;


// Thresholding

Thresholding::Thresholding(const int thr_min_, const int thr_max_) {
    thr_min = thr_min_;
    thr_max = thr_max_;
}

MatrixXi Thresholding::transform(const MatrixXi& item) {
    int nrows = item.rows();
    int ncols = item.cols();

    MatrixXi thr_item(nrows, ncols);

    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            if (item(i, j) < thr_min)
                thr_item(i, j) = thr_min;
            else if (item(i, j) > thr_max)
                thr_item(i, j) = thr_max;
            else
                thr_item(i, j) = item(i, j);
        }
    }

    return thr_item;
}


// Histogram

Histogram::Histogram() = default;

MatrixXd Histogram::transform(const MatrixXi& item) {
    int nrows = item.rows();
    int ncols = item.cols();
    int size = nrows * ncols;

    int min_val = item.minCoeff();
    int max_val = item.maxCoeff();

    MatrixXd hist = MatrixXd::Zero(1,max_val - min_val + 1);

    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            hist(0, item(i, j) - min_val) += 1. / size;
        }
    }

    return hist;
}