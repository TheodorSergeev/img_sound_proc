#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include "Eigen/Dense"
#include "Transform.hpp"
//#include <opencv2/opencv.hpp>
//#include <opencv2/imgcodecs.hpp>

//using cv::imread;

MatrixXi Thresholding::transform(const MatrixXi &item)
{
    int nrows = item.rows();
    int ncols = item.cols();

    MatrixXi thr_item(nrows, ncols);
    if (thr_min >= thr_max){
        try{throw std::invalid_argument("Incorrect parameter order, swapped");}
        catch(std::invalid_argument &e){std::cout<<e.what()<<"\n";}
        int temp = thr_min;
        thr_min = thr_max;
        thr_max = temp;
    }
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
MatrixXd Histogram::transform(const MatrixXi &item) {
    int nrows = item.rows();
    int ncols = item.cols();
    int size = nrows * ncols;

    int min_val = item.minCoeff();
    int max_val = item.maxCoeff();

    MatrixXd hist = MatrixXd::Zero(1,max_val - min_val + 1);

    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            hist(0,item(i, j) - min_val) += 1. / size;
        }
    }

    return hist;
}