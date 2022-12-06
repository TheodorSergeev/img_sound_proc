#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include "Eigen/Dense"
//#include <opencv2/opencv.hpp>
//#include <opencv2/imgcodecs.hpp>

//using cv::imread;

//#include <CImg.h>
//using namespace cimg_library;

using std::cout;
using std::string;
using std::vector;
using Eigen::MatrixXi;
using Eigen::MatrixXd;


template <typename TInput, typename TOutput>
class Transform {
public:
    virtual TOutput transform(const TInput& item_) = 0;
};

class Thresholding: public Transform<MatrixXi, MatrixXi> {
private:
    int thr_min, thr_max;

public:
    explicit Thresholding(const int thr_min_, const int thr_max_) {
        thr_min = thr_min_;
        thr_max = thr_max_;
    }

    MatrixXi transform(const MatrixXi& item) override {
        int nrows = item.rows();
        int ncols = item.cols();

        MatrixXi thr_item(nrows, ncols);

        for (int i = 0; i < nrows; ++i) {
            for (int j = 0; j < ncols; ++j) {
                if (item(i, j) < thr_min)
                    thr_item(i, j) = thr_min;
                else if (item(i, j) > thr_max)
                    thr_item(i, j) = thr_max;
                else
                    thr_item(i, j) = item(i, j);
            }
        }

        return thr_item;
    };
};

class Histogram: public Transform<MatrixXi, MatrixXd> {
public:
    explicit Histogram() = default;

    MatrixXd transform(const MatrixXi& item) override {
        int nrows = item.rows();
        int ncols = item.cols();
        int size = nrows * ncols;

        int min_val = item.minCoeff();
        int max_val = item.maxCoeff();

        MatrixXd hist = MatrixXd::Zero(1,max_val - min_val + 1);

        for (int i = 0; i < nrows; ++i) {
            for (int j = 0; j < ncols; ++j) {
                hist(0,item(i, j) - min_val) += 1. / size;
            }
        }

        return hist;
    };
};


template <typename TInput, typename TOutput>
class Parser {
protected:
    int arg_num;
    string name;

public:
    string get_name() {
        return name;
    }

    int get_arg_num() {
        return arg_num;
    }

    virtual const Transform<TInput, TOutput>* parse(vector <string> arguments) = 0;
};

class ThresholdingParser: public Parser<MatrixXi, MatrixXi> {
public:
    ThresholdingParser() {
        arg_num = 2;
        name = "threshold";
    }

    Transform<MatrixXi, MatrixXi>* parse(vector <string> arguments) override {
        cout << "n_args = " << arguments.size() << "\n";

        if (arguments.size() != arg_num)
            throw std::invalid_argument("Thresholding requires two integer arguments.");

        int thr_min = std::stoi(arguments[0]); // throws an exception if not an int
        int thr_max = std::stoi(arguments[1]); // throws an exception if not an int

        return new Thresholding(thr_min, thr_max);
    }
};


int main() { //int argc, char* argv[]) {
    //Matrix<int, -1, 1> img;
    MatrixXi img(3,3);
    img << 1, 2, 3,
           4, 5, 6,
           7, 8, 9;

    const char* HELP_STR = "--help";
    const char* HELP_MSG = "asdasd";

    int argc = 4;
    const char* argv[] = {"path", "threshold", "3", "6"};
    // ./img_sound_proc input.png output.txt threshold 3 9

    std::array parsers_list = {ThresholdingParser()};
    string opt_str(argv[1]);

    if (argc == 1) {
        cout << "Not enough arguments. Use \"./img_sound-proc --help\" to access the options.\n";
    } else {
        vector<string> arg_vec;
        for (int i = 2; i < argc; ++i) {
            arg_vec.emplace_back(argv[i]);
        }

        if (opt_str == HELP_STR) {
            cout << HELP_MSG << "\n";
        } else {
            for (auto &parser: parsers_list) {
                if (opt_str == parser.get_name()) {
                    cout << "found " << parser.get_name() << "\n";
                    auto transformer = parser.parse(arg_vec);
                    auto new_img = transformer->transform(img);
                    cout << new_img << "\n";
                }
            }
        }
    }

    /*

    cout << "Original\n" << img << "\n";

    Thresholding threshold(3,6);
    cout << "Thresholding\n" << threshold.transform(img) << "\n";

    Histogram hist;
    cout << "Histogram\n" << hist.transform(img);*/

    //string img_path("cameraman.tif");
    //CImg<float> ImgTest;
    //ImgTest.load_tiff(img_path.c_str());

    //cv::Mat image;// = cv::imread(img_path, cv::IMREAD_COLOR);

    return 0;
}
