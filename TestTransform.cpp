#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <exception>
#include "Eigen/Dense"
#include "Transform.hpp"
#include "CustomException.h"
#include "gtest/gtest.h"
//#include <opencv2/opencv.hpp>
//#include <opencv2/imgcodecs.hpp>

//using cv::imread;
class TransformTest: public ::testing::Test{
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

TEST_F(TransformTest, THRESHOLDING){
    Thresholding thre_1(5,3);
    Thresholding thre_2(12,21);
    /// Check exception thrown
    EXPECT_THROW(thre_1.transform(item_1),invalid_argument);

    MatrixXi result(2,2);
    result = thre_2.transform(item_1);
    EXPECT_EQ(result(0,0),12);
    EXPECT_EQ(result(0,1),12);
    EXPECT_EQ(result(1,0),21);
    EXPECT_EQ(result(1,1),21);
}

TEST_F(TransformTest, HISTOGRAM){
    Histogram his;
    his.transform(item_2);

/// Check return size
    EXPECT_EQ(his.mHist.cols(),4);

    ASSERT_NEAR(his.mHist(0,0), 0.25, 1e-10);
    ASSERT_NEAR(his.mHist(0,1), 0.125, 1e-10);
    ASSERT_NEAR(his.mHist(0,2), 0.125, 1e-10);
    ASSERT_NEAR(his.mHist(0,3), 0.5, 1e-10);
}

TEST_F(TransformTest, FFT1DTEST){
    FFT1D fft;
/// Check exception thrown
    EXPECT_THROW(fft.getMagnitude(),logic_error);

    fft.transform(item_1);
    EXPECT_EQ(fft.mfrequencyDomain(0),std::complex<double>(33,0));
    EXPECT_EQ(fft.mfrequencyDomain(1),std::complex<double>(-5,5));
    EXPECT_EQ(fft.mfrequencyDomain(2),std::complex<double>(-1,0));
    EXPECT_EQ(fft.mfrequencyDomain(0),std::complex<double>(-5,-5));
}

TEST_F(TransformTest, FFT1DANDINVERSE){
    FFT1D fft;
    iFFT1D ifft;
    MatrixXi refer;
    refer.resize(1,4);
    refer << 11, 12, 21, 22;
    for (int i = 0; i <4; i++){
        EXPECT_EQ(ifft.transform(fft.transform(item_1))(i),refer(i));
    }
}

TEST_F(TransformTest, FFT2DTEST){
    FFT2D fft;
    /// Check exception thrown
    EXPECT_THROW(fft.getMagnitude(),logic_error);


    EXPECT_EQ(fft.transform(item_1)(0,0),std::complex<double>(33,0));
    EXPECT_EQ(fft.transform(item_1)(0,1),std::complex<double>(-1,0));
    EXPECT_EQ(fft.transform(item_1)(1,0),std::complex<double>(-10,0));
    EXPECT_EQ(fft.transform(item_1)(1,1),std::complex<double>(0,0));
}

TEST_F(TransformTest, FFT2DANDINVERSE){
    FFT2D fft;
    iFFT2D ifft;
    MatrixXi refer;
    refer.resize(2,2);
    refer = item_1;
    for (int i = 0; i <2; i++){
        for (int j = 0; j <2; j++){
            EXPECT_EQ(ifft.transform(fft.transform(item_1))(i,j),refer(i,j));
        }
    }
}

TEST_F(TransformTest, LOWPASSFILTER){
    LowpassFilter testLow_1(1);
    LowpassFilter testLow_2(5);

    ///test filtered
    EXPECT_EQ(testLow_1(item_1)(0,0),-6);
    EXPECT_EQ(testLow_1(item_1)(0,1),-5);
    EXPECT_EQ(testLow_1(item_1)(1,0),5);
    EXPECT_EQ(testLow_1(item_1)(1,1),6);

    ///test nonfiltered
    EXPECT_EQ(testLow_2(item_1)(0,0),11);
    EXPECT_EQ(testLow_2(item_1)(0,1),12);
    EXPECT_EQ(testLow_2(item_1)(1,0),21);
    EXPECT_EQ(testLow_2(item_1)(1,1),22);
}

TEST_F(TransformTest, HIGHPASSFILTER){
    HighpassFilter testHigh_1(1);
    HighpassFilter testHigh_2(5);

    ///test nonfiltered
    EXPECT_EQ(testHigh_1(item_1)(0,0),11);
    EXPECT_EQ(testHigh_1(item_1)(0,1),12);
    EXPECT_EQ(testHigh_1(item_1)(1,0),21);
    EXPECT_EQ(testHigh_1(item_1)(1,1),22);

    ///test filtered
    EXPECT_EQ(testHigh_2(item_1)(0,0),17);
    EXPECT_EQ(testHigh_2(item_1)(0,1),17);
    EXPECT_EQ(testHigh_2(item_1)(1,0),17);
    EXPECT_EQ(testHigh_2(item_1)(1,1),17);
}