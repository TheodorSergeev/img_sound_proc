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
 * @brief Fast Fourier transform in 1D
 * 
 */
class FFT1D: public Transform<MatrixXi, Eigen::Matrix<std::complex<double>,1, Dynamic>> {
private:
    Eigen::Matrix<std::complex<double>,1, Dynamic> mfrequencyDomain; /// frequency of the transform
    Eigen::Matrix<std::complex<double>,1, Dynamic> mMagnitude; /// magnitude of the transform
    int step; /// number of steps
    int transformed; /// flag if the transform has been applied

public:
    FFT1D(int n = 1);

    /**
     * @brief Implementation of the FFT1D transform.
     *
     * @return Eigen complex matrix for the Fourier transform.
     */
    Eigen::Matrix<std::complex<double>,1, Dynamic> transform(const MatrixXi& item) override;

    /**
     * @brief Get the Magnitude object
     * 
     * @return Eigen::Matrix<std::complex<double>,1, Dynamic> Magnitude matrix
     */
    Eigen::Matrix<std::complex<double>,1, Dynamic> getMagnitude();
};


/**
 * @brief Inverse Fast Fourier transform in 1d
 * 
 */
class iFFT1D: public Transform<Eigen::Matrix<std::complex<double>,1, -1>, Eigen::Matrix<int,1, -1>> {
private:
    Eigen::Matrix<int,1, Dynamic> mspatialDomain; /// transform results in the spatial domain
    int transformed = 0; /// flag if the transform has been applied

public:
    /**
    * @brief Implementation of the inverse Fast Fourier transform in 1D
    * 
    * @param item Input matrix in Fourier domain
    * @return Eigen::Matrix<int,1, Dynamic> Output matrix in spatial domain
    */
    Eigen::Matrix<int,1, Dynamic> transform(const Eigen::Matrix<std::complex<double>,1, -1>& item) override;
};


/**
 * @brief Fast Fourier Fourier transform in 2d
 * 
 */
class FFT2D: public Transform<MatrixXi, Eigen::Matrix<std::complex<double>,-1, -1>> {
private:
    Eigen::Matrix<std::complex<double>,-1, -1> mfrequencyDomain; /// frequency of the transform
    Eigen::Matrix<std::complex<double>,-1, -1> mMagnitude; /// magnitude of the transform
    int step; /// number of steps
    int transformed; /// flag if the transform has been applied

public:
    /**
     * @brief Construct a new FFT2D object with the specified number of steps
     * 
     * @param n Number of steps
     */
    FFT2D(int n = 1);

    /**
     * @brief Implementation of the FFT1D transform.
     *
     * @return Eigen complex matrix for the Fourier transform.
     */
    Eigen::Matrix<std::complex<double>,-1, -1> transform(const MatrixXi& item) override;

    /**
     * @brief Get the Magnitude object
     * 
     * @return Eigen::Matrix<std::complex<double>,1, Dynamic> Magnitude matrix
     */
    Eigen::Matrix<std::complex<double>,-1, -1> getMagnitude();
};


/**
 * @brief Inverse Fast Fourier transform in 2d
 * 
 */
class iFFT2D: public Transform<Eigen::Matrix<std::complex<double>,-1, -1>, Eigen::Matrix<int,-1, -1>> {
private:
    Eigen::Matrix<int,-1, -1> mspatialDomain; /// transform results in the spatial domain
    int transformed = 0; /// flag if the transform has been applied

public:
    /**
     * @brief Implementation of the inverse Fast Fourier transform in 1D
     * 
     * @param item Input matrix in Fourier domain
     * @return Eigen::Matrix<int,1, Dynamic> Output matrix in spatial domain
     */
    Eigen::Matrix<int,-1, -1> transform(const Eigen::Matrix<std::complex<double>,-1, -1>& item) override;
};


/**
 * @brief Lowpass filter using 2d Fourier trasnforms
 * 
 */
class LowpassFilter : public Transform<MatrixXi,MatrixXi> {
private:
    double thr; /// threshold for the filter
    int stp; /// number of steps for the FFT
    MatrixXi filtered; /// the filtered matrix
    int transformed; /// flag if the transform has been applied

public:
    /**
     * @brief Construct a new Lowpass Filter object
     * 
     * @param threshold Threshold value
     * @param step Number of steps for the FFT
     */
    explicit LowpassFilter(const double threshold, int step = 1);

    /**
     * @brief Apply the low pass flter
     * 
     * @param item Eigen matrix before filtering
     * @return MatrixXi Eigen matrix after filtering
     */
    MatrixXi transform(const MatrixXi& item) override;
};


/**
 * @brief Highpass filter using 2d Fourier trasnforms
 * 
 */
class HighpassFilter : public Transform<MatrixXi,MatrixXi> {
private:
    double thr; /// threshold for the filter
    int stp; /// number of steps for the FFT
    MatrixXi filtered; /// the filtered matrix
    int transformed; /// flag if the transform has been applied

public:
    /**
     * @brief Construct a new Lowpass Filter object
     * 
     * @param threshold Threshold value
     * @param step Number of steps for the FFT
     */
    explicit HighpassFilter(const double threshold, int step = 1);
    
    /**
     * @brief Apply the low pass flter
     * 
     * @param item Eigen matrix before filtering
     * @return MatrixXi Eigen matrix after filtering
     */
    MatrixXi transform(const MatrixXi& item) override;
};

#endif