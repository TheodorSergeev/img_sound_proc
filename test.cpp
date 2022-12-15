#include <gtest/gtest.h>
#include "utils.hpp"
#include "transforms.hpp"
#include "parsing.hpp"

using Eigen::MatrixXi;
using Eigen::MatrixXd;


// ==================================================================================
// test utils
// ==================================================================================

/**
 * @brief Check correctness of opencv2eigen (size and contents).
 * 
 */
TEST(UtilsTest, opencv2eigen) {
    cv::Mat image(2, 3, CV_8U, 4);
    MatrixXi test_mat = opencv2eigen(image);

    // Check matrix size
    EXPECT_EQ(test_mat.cols(), 3);
    EXPECT_EQ(test_mat.rows(), 2);

    // Check matrix contents
    EXPECT_EQ(test_mat.minCoeff(), 4);
    EXPECT_EQ(test_mat.maxCoeff(), 4);
}

/**
 * @brief Check correctness of eigen2opencv (size and contents).
 * 
 */
TEST(UtilsTest, eigen2opencv) {
    MatrixXi image(2, 3);
    image << 4, 4, 4, 
             4, 4, 4;
    cv::Mat test_mat = eigen2opencv(image);

    // Check matrix size
    EXPECT_EQ(test_mat.cols, 3);
    EXPECT_EQ(test_mat.rows, 2);

    // Check matrix contents
    double test_min, test_max;
    cv::minMaxLoc(test_mat, &test_min, &test_max);
    EXPECT_EQ(test_min, 4);
    EXPECT_EQ(test_max, 4);
}


// ==================================================================================
// test transforms
// ==================================================================================

/**
 * @brief Setup two test matrices to check correctness of transforms.
 * 
 */
class TransformTest: public::testing::Test{
protected:
    MatrixXi item_1;
    MatrixXi item_2;
    void SetUp() override{
        item_1.resize(2,2);
        item_1 << 11, 12,
                  21, 22;
        item_2.resize(2,4);
        item_2 << 1, 1, 2, 3,
                  4, 4, 4, 4;
    }
};

/**
 * @brief Check correctness of the thresholding transform
 * 
 */
TEST_F(TransformTest, THRESHOLDING){
    Thresholding thre_1(5,3);
    Thresholding thre_2(12,21);
    /// Check exception thrown
    EXPECT_THROW(thre_1.transform(item_1), std::invalid_argument);

    MatrixXi result(2,2);
    result = thre_2.transform(item_1);
    EXPECT_EQ(result(0, 0), 12);
    EXPECT_EQ(result(0, 1), 12);
    EXPECT_EQ(result(1, 0), 21);
    EXPECT_EQ(result(1, 1), 21);
}

/**
 * @brief Check correctness of the histogram transform
 * 
 */
TEST_F(TransformTest, HISTOGRAM){
    Histogram his;
    his.transform(item_2);

    /*
    /// Check return size
    EXPECT_EQ(his.mHist.cols(),4);

    ASSERT_NEAR(his.mHist(0,0), 0.25, 1e-10);
    ASSERT_NEAR(his.mHist(0,1), 0.125, 1e-10);
    ASSERT_NEAR(his.mHist(0,2), 0.125, 1e-10);
    ASSERT_NEAR(his.mHist(0,3), 0.5, 1e-10);
    */
}

/**
 * @brief Check correctness of 1d Fourier transform
 * 
 */
TEST_F(TransformTest, FFT1DTEST){
    FFT1D fft;
    /// Check exception thrown
    EXPECT_THROW(fft.getMagnitude(), std::logic_error);

    fft.transform(item_1);
    auto mat = fft.getMagnitude();
    EXPECT_EQ(mat(0), std::complex<double>(33, 0));
    EXPECT_EQ(mat(1), std::complex<double>(-5, 5));
    EXPECT_EQ(mat(2), std::complex<double>(-1, 0));
    EXPECT_EQ(mat(0), std::complex<double>(-5, -5));
}

/**
 * @brief Check correctness of 1d inverse Fourier transform
 * 
 */
TEST_F(TransformTest, FFT1DANDINVERSE){
    FFT1D fft;
    iFFT1D ifft;
    MatrixXi refer;
    refer.resize(1,4);
    refer << 11, 12, 21, 22;
    auto res = ifft.transform(fft.transform(item_1));

    for (int i = 0; i < 4; i++){
        EXPECT_EQ(res(i), refer(i));
    }
}

/**
 * @brief Check correctness of 2d Fourier transform
 * 
 */
TEST_F(TransformTest, FFT2DTEST){
    FFT2D fft;
    /// Check exception thrown
    EXPECT_THROW(fft.getMagnitude(), std::logic_error);

    auto mat = fft.transform(item_1);

    EXPECT_EQ(mat(0,0), std::complex<double>(33, 0));
    EXPECT_EQ(mat(0,1), std::complex<double>(-1, 0));
    EXPECT_EQ(mat(1,0), std::complex<double>(-10, 0));
    EXPECT_EQ(mat(1,1), std::complex<double>(0, 0));
}

/**
 * @brief Check correctness of 2d inverse Fourier transform
 * 
 */
TEST_F(TransformTest, FFT2DANDINVERSE){
    FFT2D fft;
    iFFT2D ifft;
    MatrixXi refer;
    refer.resize(2, 2);
    refer = item_1;
    auto res = ifft.transform(fft.transform(item_1));

    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            EXPECT_EQ(res(i, j), refer(i, j));
        }
    }
}

/**
 * @brief Check correctness of the lowpass filter
 * 
 */
TEST_F(TransformTest, LOWPASSFILTER){
    LowpassFilter testLow_1(1);
    LowpassFilter testLow_2(5);

    /// test filtered
    auto mat1 = testLow_1.transform(item_1);
    EXPECT_EQ(mat1(0, 0), -6);
    EXPECT_EQ(mat1(0, 1), -5);
    EXPECT_EQ(mat1(1, 0), 5);
    EXPECT_EQ(mat1(1, 1), 6);

    /// test nonfiltered
    auto mat2 = testLow_1.transform(item_1);
    EXPECT_EQ(mat2(0, 0), 11);
    EXPECT_EQ(mat2(0, 1), 12);
    EXPECT_EQ(mat2(1, 0), 21);
    EXPECT_EQ(mat2(1, 1), 22);
}

/**
 * @brief Check correctness of the high pass filter
 * 
 */
TEST_F(TransformTest, HIGHPASSFILTER){
    HighpassFilter testHigh_1(1);
    HighpassFilter testHigh_2(5);

    /// test nonfiltered
    auto mat1 = testHigh_1.transform(item_1);
    EXPECT_EQ(mat1(0, 0), 11);
    EXPECT_EQ(mat1(0, 1), 12);
    EXPECT_EQ(mat1(1, 0), 21);
    EXPECT_EQ(mat1(1, 1), 22);

    /// test filtered
    auto mat2 = testHigh_2.transform(item_1);
    EXPECT_EQ(mat2(0, 0), 17);
    EXPECT_EQ(mat2(0, 1), 17);
    EXPECT_EQ(mat2(1, 0), 17);
    EXPECT_EQ(mat2(1, 1), 17);
}


// ==================================================================================
// test parsers
// ==================================================================================
