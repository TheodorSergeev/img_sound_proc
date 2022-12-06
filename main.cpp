#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include "Eigen/Dense"
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <filesystem>

using cv::imread;

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
    string name;

public:
    string get_name() {
        return name;
    }

    virtual Transform<TInput, TOutput>* parse(vector <string> arguments) = 0;
};

class ThresholdingParser: public Parser<MatrixXi, MatrixXi> {
public:
    ThresholdingParser() {
        name = "threshold";
    }

    Transform<MatrixXi, MatrixXi>* parse(vector <string> arguments) override {
        const int n_args = 2;

        if (arguments.size() != n_args)
            throw std::invalid_argument("Thresholding requires two integer arguments.");
                    
        // throw an exception if not convertible to int
        int thr_min = std::stoi(arguments[0]);
        int thr_max = std::stoi(arguments[1]);

        return new Thresholding(thr_min, thr_max);
    }
};


int main() { //int argc, char* argv[]) {
    const char* HELP_STR = "--help";
    const char* HELP_MSG = "asdasd";

    vector <const char*> argv = {"path", "threshold", "inp.tif", "out.png", "3", "6"};
    int argc = argv.size();

    std::array parsers_list = {ThresholdingParser()};

    if (argc == 1) {
        cout << "Not enough arguments. Use \"./img_sound-proc --help\" to access the options.\n";
    } else {
        string opt_str(argv[1]);

        cout << opt_str << "\n";

        if (opt_str == HELP_STR) {
            cout << HELP_MSG << "\n";
        } else {
            for (auto & parser : parsers_list) {
                if (opt_str == parser.get_name()) {
                    if (argc < 4) {
                        cout << "not enough parameters\n";
                    } else {
                        cout << "found " << parser.get_name() << "\n";

                        string inp_fname = argv[2];
                        string out_fname = argv[3];

                        vector<string> arg_vec;
                        for (int i = 4; i < argc; ++i) {
                            arg_vec.emplace_back(argv[i]);
                        }
                        
                        // auto inp_img = read(inp_fname);

                        MatrixXi inp_img(3,3);
                        inp_img << 1, 2, 3,
                                   4, 5, 6,
                                   7, 8, 9;
                        
                        auto transformer = parser.parse(arg_vec);
                        auto out_img = transformer->transform(inp_img);
                        cout << out_img << "\n";
                        // write(out_fname);
                    }  
                    break;
                }
            }
        }
    }

    /*MatrixXi img(3,3);
    img << 1, 2, 3,
           4, 5, 6,
           7, 8, 9;

    cout << "Original\n" << img << "\n";

    Thresholding threshold(3,6);
    cout << "Thresholding\n" << threshold.transform(img) << "\n";

    Histogram hist;
    cout << "Histogram\n" << hist.transform(img);*/

    /*string rel_path("/data/images/cameraman.tif");
    string abs_path(std::filesystem::current_path());
    string img_path(abs_path + rel_path);

    cout << img_path << "\n";
    cv::Mat image = cv::imread(img_path, cv::IMREAD_GRAYSCALE);*/

    return 0;
}
