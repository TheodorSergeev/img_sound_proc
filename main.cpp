#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include "Eigen/Dense"
#include "Transform.hpp"
//#include <opencv2/opencv.hpp>
//#include <opencv2/imgcodecs.hpp>

//using cv::imread;

using std::cout;
using std::string;
using std::vector;
using Eigen::MatrixXi;
using Eigen::MatrixXd;


template <typename TInput, typename TOutput>
class Parser {
protected:
    string name;

public:
    string get_name() {
        return name;
    }

    virtual const Transform<TInput, TOutput>* parse(vector <string> arguments) = 0;
};

class ThresholdingParser: public Parser<MatrixXi, MatrixXi> {
public:
    ThresholdingParser() {
        name = "threshold";
    }

    const Transform<MatrixXi, MatrixXi>* parse(vector <string> arguments) override {
        const int n_args = 2;

        if (arguments.size() != n_args)
            throw std::invalid_argument("Thresholding requires two integer arguments.");

        int thr_min = std::stoi(arguments[0]); // throws an exception if not an int
        int thr_max = std::stoi(arguments[1]); // throws an exception if not an int

        return new Thresholding(thr_min, thr_max);
    }
};


int main() { //int argc, char* argv[]) {
    //Matrix<int, -1, 1> img;

    const char* HELP_STR = "--help";
    const char* HELP_MSG = "asdasd";

    int argc = 4;
    const char* argv[] = {"path", "threshold", "3", "9"};

    std::array parsers_list = {ThresholdingParser()};

    if (argc == 1) {
        cout << "Not enough arguments. Use \"./img_sound-proc --help\" to access the options.\n";
    } else {
        string opt_str(argv[1]);
        cout << opt_str << "\n";

        vector<string> arg_vec;
        for (int i = 2; i < argc; ++i) {
            arg_vec.emplace_back(argv[i]);
        }

        if (opt_str == HELP_STR) {
            cout << HELP_MSG << "\n";
        }

        for (auto & parser : parsers_list) {
            if (opt_str == parser.get_name()) {
                cout << "found " << parser.get_name() << "\n";
                //pars.parse();
            }
        }
    }

   /* MatrixXi img(3,3);
    img << 1, 2, 3,
           4, 5, 6,
           7, 8, 9;

    cout << "Original\n" << img << "\n";

    Thresholding threshold(1,3);
    cout << "Thresholding\n" << threshold.transform(img) << "\n";


    Histogram hist;
    cout << "Histogram\n" << hist.transform(img);

    string img_path("/data/images/cameraman.tif");*/
    //cv::Mat image = cv::imread(img_path, cv::IMREAD_COLOR);

    return 0;
}
