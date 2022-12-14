#ifndef TRANSFORMS
#define TRANSFORMS

#include <vector>
#include <exception>
#include "Eigen/Dense"
#include <cmath>
#include <complex>
#include <string>

#include <iostream> // shouldn't be here as IO is covered by parsers
#include <fstream> // shouldn't be here as IO is covered by parsers

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

public:
    explicit Thresholding(const int thr_min_, const int thr_max_);

    MatrixXi transform(const MatrixXi& item) override;
};


class Histogram: public Transform<MatrixXi, MatrixXd> {
public:
    explicit Histogram();

    MatrixXd transform(const MatrixXi& item) override;
};



void mainFFT1D(std::complex<float> signal[], int start, int fin, int step1, float inv, std::complex<float> buffer[]);

void normalize(std::complex<float> f[], int n, float norm);

class FFT1D: public Transform<MatrixXi, Eigen::Matrix<std::complex<double>,1, Dynamic>> {
private:
    Eigen::Matrix<std::complex<double>,1, Dynamic> mfrequencyDomain;
    Eigen::Matrix<std::complex<double>,1, Dynamic> mMagnitude;
    int step;
    int transformed =0;
public:
    FFT1D( int n = 1){
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
    void save_freq(std::string filename = "unspecified item");
    void save_mag(std::string filename = "unspecified item");

};


class iFFT1D: public Transform<Eigen::Matrix<std::complex<double>,1, -1>, Eigen::Matrix<int,1, -1>> {
private:
    Eigen::Matrix<int,1, Dynamic> mspatialDomain;
    int transformed = 0;
public:
    explicit iFFT1D() = default;
    Eigen::Matrix<int,1, Dynamic> transform(const Eigen::Matrix<std::complex<double>,1, -1>& item) override;
    void save(std::string filename = "unspecified item");

};


class FFT2D: public Transform<MatrixXi, Eigen::Matrix<std::complex<double>,-1, -1>> {
private:
    Eigen::Matrix<std::complex<double>,-1, -1> mfrequencyDomain;
    Eigen::Matrix<std::complex<double>,-1, -1> mMagnitude;
    int step;
    int transformed =0;

public:

    FFT2D(int n = 1){
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
    void save_freq(std::string filename = "unspecified item");
    void save_mag(std::string filename = "unspecified item");
};


class iFFT2D: public Transform<Eigen::Matrix<std::complex<double>,-1, -1>, Eigen::Matrix<int,-1, -1>> {
private:
    Eigen::Matrix<int,-1, -1> mspatialDomain;
    int transformed = 0;

public:
    explicit iFFT2D() = default;
    Eigen::Matrix<int,-1, -1> transform(const Eigen::Matrix<std::complex<double>,-1, -1>& item) override;
    void save(std::string filename = "unspecified item");
};


class LowpassFilter : public Transform<MatrixXi,MatrixXi> {
private:
    double thr;
    int stp;
    MatrixXi filtered;
    int transformed = 0;

public:
    explicit LowpassFilter(const double threshold, int step = 1) {
        thr = threshold;
        stp = step;
    }
    MatrixXi transform(const MatrixXi& item) override;
    void save(std::string filename = "unspecified item");
};


class HighpassFilter : public Transform<MatrixXi,MatrixXi> {
private:
    double thr;
    int stp;
    MatrixXi filtered;
    int transformed = 0;

public:
    explicit HighpassFilter(const double threshold, int step = 1) {
        thr = threshold;
        stp = step;
    }
    MatrixXi transform(const MatrixXi& item) override;
    void save(std::string filename = "unspecified item");
};

#endif