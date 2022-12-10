#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include "Eigen/Dense"
#include <filesystem>
#include "utils.hpp"


using std::cout;
using std::string;
using std::vector;
using std::to_string;
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

    void checkArgNum(const vector<string>& arguments) {
        if (arguments.size() != 2 + get_arg_num()) {
            throw std::invalid_argument(
                "Incorect number of parameters for " + get_name() + \
                "\nRequired: " + to_string(2 + get_arg_num()) + \
                "\nProvided: " + to_string(arguments.size() - 2) + "\n"
            );
        }
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


void parse_cl_input(int argc, const char* argv[]) {
    const char* HELP_STR = "--help";
    const char* HELP_MSG = "asdasd";
    std::array parsers_list = {ThresholdingParser()}; // pass as an argument?

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
}


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
    cout << "Original\n" << img << "\n";

    Thresholding threshold(3,6);
    cout << "Thresholding\n" << threshold.transform(img) << "\n";

    Histogram hist;
    cout << "Histogram\n" << hist.transform(img);*/

    return 0;
}
