#ifndef TRANSFORMHEADERDEF
#define TRANSFORMHEADERDEF
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <exception>
#include <complex>
#include "Eigen/Dense"
//#include <opencv2/opencv.hpp>
//#include <opencv2/imgcodecs.hpp>

//using cv::imread;
using std::cout;
using std::string;
using std::vector;
using Eigen::MatrixXi;
using Eigen::MatrixXd;
using Eigen::Dynamic;

template <typename TInput, typename TOutput>
class Transform {
public:
    virtual TOutput transform(const TInput& item_) = 0;
};


class Thresholding: public Transform<MatrixXi, MatrixXi> {
private:
    int thr_min, thr_max;
    MatrixXi mthr_item;
    int transformed = 0;
public:
    explicit Thresholding(const int thr_min_, const int thr_max_) {
        thr_min = thr_min_;
        thr_max = thr_max_;
    }

    MatrixXi transform(const MatrixXi& item) override;
    void save(std::string filename = "unspecified item");
};

class Histogram: public Transform<MatrixXi, MatrixXd> {
private:
    MatrixXd mHist;
    int transformed = 0;
public:
    explicit Histogram() = default;
    MatrixXd transform(const MatrixXi& item) override;
    void save(std::string filename = "unspecified item");
};

class FFT1D: public Transform<MatrixXi, Eigen::Matrix<std::complex<double>,1, Dynamic>> {
private:
    Eigen::Matrix<std::complex<double>,1, Dynamic> mfrequencyDomain;
    Eigen::Matrix<std::complex<double>,1, Dynamic> mMagnitude;
    int step;
    int transformed =0;
public:
    explicit FFT1D() = default;
    explicit FFT1D(const int n){
        step = n;
    }
    Eigen::Matrix<std::complex<double>,1, Dynamic> transform(const MatrixXi& item) override;
    Eigen::Matrix<std::complex<double>,1, Dynamic> getMagnitude(){
        if (transformed != 1){
            try {
                throw std::logic_error("perform transform first");
            }
            catch (const std::logic_error &e) {
                std::cout << e.what() << std::endl;
            }
        }
        return mMagnitude;

    }

};

class iFFT1D: public Transform<Eigen::Matrix<std::complex<double>,1, -1>, Eigen::Matrix<int,1, -1>> {
private:
    Eigen::Matrix<int,1, Dynamic> mspatialDomain;
public:
    explicit iFFT1D() = default;
    Eigen::Matrix<int,1, Dynamic> transform(const Eigen::Matrix<std::complex<double>,1, -1>& item) override;

};

class FFT2D: public Transform<MatrixXi, Eigen::Matrix<std::complex<double>,-1, -1>> {
private:
    Eigen::Matrix<std::complex<double>,-1, -1> mfrequencyDomain;
    Eigen::Matrix<std::complex<double>,-1, -1> mMagnitude;
    int step;
    int transformed =0;
public:
    explicit FFT2D() = default;
    explicit FFT2D(const int n){
        step = n;
    }
    Eigen::Matrix<std::complex<double>,-1, -1> transform(const MatrixXi& item) override;
    Eigen::Matrix<std::complex<double>,-1, -1> getMagnitude(){
        if (transformed != 1){
            try {
                throw std::logic_error("perform transform first");
            }
            catch (const std::logic_error &e) {
                std::cout << e.what() << std::endl;
            }
        }
        return mMagnitude;

    }

};

class iFFT2D: public Transform<Eigen::Matrix<std::complex<double>,-1, -1>, Eigen::Matrix<int,-1, -1>> {
private:
    Eigen::Matrix<int,-1, -1> mspatialDomain;
public:
    explicit iFFT2D() = default;
    Eigen::Matrix<int,-1, -1> transform(const Eigen::Matrix<std::complex<double>,-1, -1>& item) override;

};

class LowpassFilter : public Transform<MatrixXi,MatrixXi> {
private:
    double thr;
    int stp;
public:
    explicit LowpassFilter(const double threshold, int step = 1) {
        thr = threshold;
        step = 1;
    }
    MatrixXi transform(const MatrixXi& item) override;
};
class HighpassFilter : public Transform<MatrixXi,MatrixXi> {
private:
    double thr;
    int stp;
public:
    explicit HighpassFilter(const double threshold, int step = 1) {
        thr = threshold;
        stp = step;
    }
    MatrixXi transform(const MatrixXi& item) override;
};
#endif