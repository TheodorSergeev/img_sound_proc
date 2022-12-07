#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include "Eigen/Dense"
#include "AudioFile.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <filesystem>
#include "utils.hpp"


using std::cout;
using std::string;
using std::vector;
using std::to_string;
using Eigen::MatrixXi;
using Eigen::MatrixXd;
using cv::imread;


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

    void checkArgNum(const vector<string>& arguments) {
        if (arguments.size() != 2 + get_arg_num()) {
            throw std::invalid_argument(
                "Incorect number of parameters for " + get_name() + \
                "\nRequired: " + to_string(2 + get_arg_num()) + \
                "\nProvided: " + to_string(arguments.size() - 2) + "\n"
            );
        }
    }

    MatrixXi readIntMatrix(const string& inp_fname) {
        string extension(std::filesystem::path(inp_fname).extension());

        cout << inp_fname << " " << cv::haveImageReader(inp_fname) << "\n";

        if (cv::haveImageReader(inp_fname)) {
            cv::Mat image = cv::imread(inp_fname, cv::IMREAD_GRAYSCALE);
            return opencv2eigen(image);
        } else {
            throw std::invalid_argument("Cannot read an integer matrix from " + inp_fname);
        }
        // todo: check if audio can be loaded as ints

        MatrixXi tmp_inp_img;
        return tmp_inp_img;
    }

    void writeIntMatrix(const string& out_fname, const MatrixXi& matrix) {
        bool result = true;

        if (cv::haveImageWriter(out_fname)) {
            cv::Mat cv_mat = eigen2opencv(matrix);
            result = cv::imwrite(out_fname, cv_mat);
        } else {
            throw std::invalid_argument("Cannot write an integer matrix to " + out_fname);
        }
        // todo: check if audio can be loaded as ints

        if (result) {
            cout << "File saved: " << out_fname << "\n"; 
        } else {
            cout << "Can't save file: " << out_fname << "\n"; 
        }
    }


    MatrixXd readFloatMatrix(const string& inp_fname) {
        string extension(std::filesystem::path(inp_fname).extension());

        if (extension == "wav" || extension == "aiff") {
            AudioFile<float> audioFile;
            audioFile.load(inp_fname);
            // return aufiodileMat2Eigen
        } else {
            throw std::invalid_argument("Cannot read an integer matrix from " + inp_fname);
        }
        // todo: check if audio can be loaded as ints

        MatrixXd tmp_inp_img(3,3);
        tmp_inp_img << 1.0, 2.0, 3.0,
                       4.0, 5.0, 6.0,
                       7.0, 8.0, 9.0;
        return tmp_inp_img;
    }

public:
    string get_name() {
        return name;
    }

    int get_arg_num() {
        return arg_num;
    }

    virtual Transform<TInput, TOutput>* parse(const vector<string>& arguments) = 0;
};

class ThresholdingParser: public Parser<MatrixXi, MatrixXi> {
public:
    ThresholdingParser() {
        arg_num = 2;
        name = "threshold";
    }

    Transform<MatrixXi, MatrixXi>* parse(const vector<string>& arguments) override {
        cout << "n_args = " << arguments.size() << "\n";

        if (arguments.size() != 2 + arg_num)
            throw std::invalid_argument("Thresholding requires exactly two integer arguments.");
                    
        // throw an exception if not convertible to int
        int thr_min = std::stoi(arguments[2]);
        int thr_max = std::stoi(arguments[3]);

        return new Thresholding(thr_min, thr_max);
    }

    void apply(const vector <string>& arguments) {
        checkArgNum(arguments); 

        // todo: why opencv doesn't want to read from the relative path?
        string glob_path = std::filesystem::current_path();
        string inp_fname = glob_path + arguments[0];
        string out_fname = glob_path + arguments[1];

        MatrixXi input = readIntMatrix(inp_fname);
        auto thresh = parse(arguments);
        MatrixXi output = thresh->transform(input);
        writeIntMatrix(out_fname, output);
    }
};


int main() { //int argc, char* argv[]) {
    const char* HELP_STR = "--help";
    const char* HELP_MSG = "asdasd";

    vector <const char*> argv = {"path", "threshold", "/data/images/cameraman.tif", "/out.png", "30", "200"};
    int argc = argv.size();

    std::array parsers_list = {ThresholdingParser()};
    string opt_str(argv[1]);

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
                    cout << "found " << parser.get_name() << "\n";

                    vector<string> arg_vec;
                    for (int i = 2; i < argc; ++i) {
                        arg_vec.emplace_back(argv[i]);
                    }
                    
                    parser.apply(arg_vec);
                    break;
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

    /*string rel_path("/data/images/cameraman.tif");
    string abs_path(std::filesystem::current_path());
    string img_path(abs_path + rel_path); */

    return 0;
}
