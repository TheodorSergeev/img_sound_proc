#ifndef TRANSFORMS
#define TRANSFORMS

#include <vector>
#include <exception>
#include "Eigen/Dense"
#include <cmath>
#include <complex>
#include <string>
#include <iostream>
#include <fstream>

using std::vector;
using Eigen::MatrixXi;
using Eigen::MatrixXd;
using Eigen::Dynamic;

/**
 * @brief Interface of transformers (i.e. desired process for input signal).
 * The class describes how the input signal is processed
 *
 * @tparam TInput Type of the transform input (e.g., Eigen integer matrix).
 * @tparam TOutput Type of the transform output (e.g., Eigen complex matrix).
 */
template <typename TInput, typename TOutput>
class Transform {
public:
    /**
     * @brief Implementation of the transform.
     *
     * @return TOutput type Eigen matrix.
     */
    virtual TOutput transform(const TInput& item_) = 0;
};

/**
 * @brief transformer for filter thresholding the grayscale.
 */
class Thresholding: public Transform<MatrixXi, MatrixXi> {
private:
    int thr_min, thr_max; /// lower and upper threshold

public:
    /**
    * @brief Constructor for Thresholding with specified thresholding arguments
    *
    * @param thr_min lower threshold
    * @param thr_max upper threshold
    */
    explicit Thresholding(const int thr_min_, const int thr_max_);

    /**
     * @brief Implementation of the thresholding transform.
     *
     * @return Eigen integer matrix for the filtered space domain.
     */
    MatrixXi transform(const MatrixXi& item) override;
};

/**
 * @brief transformer for calculating the grayscale intensity histogram.
 */
class Histogram: public Transform<MatrixXi, MatrixXd> {
public:

    /**
    * @brief Constructor for Histogram with specified thresholding arguments
    */
    explicit Histogram();

    /**
     * @brief Implementation of the histogram transform.
     *
     * @return Eigen double matrix for the calculated intensity histogram.
     */
    MatrixXd transform(const MatrixXi& item) override;
};


/**
 * @brief mainbody of the 1D Fast Fourier Transform realized with Cooley–Tukey FFT algorithm.
 *
 * @param signal[] whole signal being transformed
 * @param start beginning position of the signal being processed at current step
 * @param fin ending position of the signal being processed at current step
 * @param step1 step of the FFT transform
 * @param inv for FFT, inv = -1, for inverse FFT, inv = 1
 * @param buffer[] float complex array for temporarily store transformed results
 */
void mainFFT1D(std::complex<float> signal[], int start, int fin, int step1, float inv, std::complex<float> buffer[]);

/**
 * @brief normalization of the FFT transformed results, assuring signal remains constant after FFT and then inverse FFT.
 *
 * @param f[] float complex array being normalized
 * @param n size of f[]
 * @param norm normalization divider
 */
void normalize(std::complex<float> f[], int n, float norm);


/**
 * @brief Fourier transform in 1D
 * 
 */
class FFT1D: public Transform<MatrixXi, Eigen::Matrix<std::complex<double>,1, Dynamic>> {
private:
    Eigen::Matrix<std::complex<double>,1, Dynamic> mfrequencyDomain;
    Eigen::Matrix<std::complex<double>,1, Dynamic> mMagnitude;
    int step;
    int transformed;

public:
    FFT1D( int n = 1){
        step = n;
        transformed = 0;
    }

    Eigen::Matrix<std::complex<double>,1, Dynamic> transform(const MatrixXi& item) override;
    Eigen::Matrix<std::complex<double>,1, Dynamic> getMagnitude();
};


class iFFT1D: public Transform<Eigen::Matrix<std::complex<double>,1, -1>, Eigen::Matrix<int,1, -1>> {
private:
    Eigen::Matrix<int,1, Dynamic> mspatialDomain;
    int transformed = 0;

public:
    explicit iFFT1D() = default;
    Eigen::Matrix<int,1, Dynamic> transform(const Eigen::Matrix<std::complex<double>,1, -1>& item) override;
};


class FFT2D: public Transform<MatrixXi, Eigen::Matrix<std::complex<double>,-1, -1>> {
private:
    Eigen::Matrix<std::complex<double>,-1, -1> mfrequencyDomain;
    Eigen::Matrix<std::complex<double>,-1, -1> mMagnitude;
    int step;
    int transformed;

public:
    FFT2D(int n = 1){
        step = n;
        transformed = 0;
    }

    Eigen::Matrix<std::complex<double>,-1, -1> transform(const MatrixXi& item) override;
    Eigen::Matrix<std::complex<double>,-1, -1> getMagnitude(){
        if (transformed == 0){
            throw std::logic_error("perform transform first");
        }
        return mMagnitude;

    }
};


class iFFT2D: public Transform<Eigen::Matrix<std::complex<double>,-1, -1>, Eigen::Matrix<int,-1, -1>> {
private:
    Eigen::Matrix<int,-1, -1> mspatialDomain;
    int transformed = 0;

public:
    explicit iFFT2D() = default;
    Eigen::Matrix<int,-1, -1> transform(const Eigen::Matrix<std::complex<double>,-1, -1>& item) override;
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
};

#endif