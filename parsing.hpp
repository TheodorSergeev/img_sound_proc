#ifndef PARSING
#define PARSING

#include <iostream>
#include <string>
#include <vector>
#include "Eigen/Dense"
#include "utils.hpp"
#include "transforms.hpp"

using std::string;
using std::to_string;
using std::vector;
using Eigen::MatrixXi;
using Eigen::MatrixXd;


void parse_cl_input(int argc, const char* argv[]);


template <typename TInput, typename TOutput>
class Parser {
protected:
    int arg_num;
    string name;

    void checkArgNum(const vector<string>& arguments);

public:
    string get_name();
    int get_arg_num();
    virtual Transform<TInput, TOutput>* parse(const vector<string>& arguments) = 0;
};


class ThresholdingParser: public Parser<MatrixXi, MatrixXi> {
public:
    ThresholdingParser();
    Transform<MatrixXi, MatrixXi>* parse(const vector<string>& arguments) override;
    void apply(const vector <string>& arguments);
};


#endif