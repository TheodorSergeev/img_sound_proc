#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include "Eigen/Dense"
#include <experimental/filesystem>
#include "utils.hpp"
#include "transforms.hpp"
#include "parsing.hpp"


using std::cout;
using std::string;
using std::vector;
using std::to_string;
using Eigen::MatrixXi;
using Eigen::MatrixXd;


int main() { //int argc, char* argv[]) { 
    vector <const char*> argv = {"path", "threshold", "/data/images/cameraman.tif", "/out.png", "30", "200.1"};
    int argc = argv.size();

    try {
        parse_cl_input(argc, argv.data());
    } catch (const std::exception& err) {
        cout << err.what() << "\n";
    } catch (...) {
        cout << "Unprocessed exception\n";
    }

    /*
>>>>>>> master
    cout << "Original\n" << img << "\n";

    Thresholding threshold(1,3);
    cout << "Thresholding\n" << threshold.transform(img) << "\n";


    Histogram hist;
    cout << "Histogram\n" << hist.transform(img);*/

    return 0;
}
