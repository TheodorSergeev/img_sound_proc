#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include "Eigen/Dense"
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
}