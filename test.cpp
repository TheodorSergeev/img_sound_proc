#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include "Eigen/Dense"
#include "Transform.hpp"
//#include <opencv2/opencv.hpp>
//#include <opencv2/imgcodecs.hpp>

//using cv::imread;

void foo() { throw std::runtime_error("my message"); }
int main() {
    try {
        foo();
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }
    Eigen::MatrixXi imag;
    imag.resize(2,4);
    imag.resize(4,4);
    imag << 0,1,2,3,
            9,8,7,6,
            4,2,5,6,
            6,8,3,1;

    FFT1D test1(1);
    std::cout <<test1.transform(imag)<<std::endl;
    FFT2D test(1);
    std::cout <<test.transform(imag)<<std::endl;
    test.getMagnitude();
    iFFT2D itest;
    itest.transform(test.transform(imag));
    std::cout <<std::endl;
    LowpassFilter testLow(1.2);
    testLow.transform(imag);
    std::cout <<std::endl;
    HighpassFilter testHigh(1.2);
    testHigh.transform(imag);

    return 0;

}