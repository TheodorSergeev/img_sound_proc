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


int main(int argc, const char* argv[]) { 
    try {
        parse_cl_input(argc, argv); //argv.data()
    } catch (const std::exception& err) {
        cout << err.what() << "\n";
    } catch (...) {
        cout << "Unprocessed exception\n";
    }
  
    return 0;
}
