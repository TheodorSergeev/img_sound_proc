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
    imag << 0,1,2,3,
            4,5,6,7;
    FFT1D test(1);
    std::cout <<test.transform(imag);
    test.getMagnitude();
    iFFT1D itest;
    itest.transform(test.transform(imag));

    return 0;

}