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


/**
 * @brief Read the command line input and call a parser for a corresponding  transform.
 * 
 * @param argc Number of command line arguments (same as argc in main).
 * @param argv Array of command line arguments (same as argv in main).
 */
void parse_cl_input(int argc, const char* argv[]);


/**
 * @brief Interface of parsers for transforms. 
 * The class describes how the command line arguments are treated,
 * how the transorm class is instantiated, and how input/output files are
 * processed.
 * 
 * @tparam TInput Type of the transform input (e.g., Eigen integer matrix).
 * @tparam TOutput Type of the transform output (e.g., Eigen float matrix).
 */
template <typename TInput, typename TOutput>
class Parser {
protected:
    int arg_num; /// Number of arguments that the transform requires.
    string name; /// Name of the transform in the command line.

    /**
     * @brief Check that enough arguments have been passed to the transform.
     * By default, arg_num+2 arguments are expected (2 is for the input and output files).
     * If the condition is not satisfied, throws an std::invalid_argument exception
     * 
     * @param arguments List of arguments (parameters of the transform) passed through the command line.
     */
    void checkArgNum(const vector<string>& arguments);

public:
    /**
     * @brief Get the name of the transform in the command line.
     * 
     * @return string The name of the transform.
     */
    string get_name();

    /**
     * @brief Get the number of argument the transform needs.
     * 
     * @return int The number of arguments.
     */
    int get_arg_num();

    /**
     * @brief Instantiate a corresponding transform with the specified parameters.
     * 
     * @param arguments List of arguments (parameters of the transform) passed through the command line.
     * @return Transform<TInput, TOutput>* Instance of the transform.
     */
    virtual Transform<TInput, TOutput>* parse(const vector<string>& arguments) = 0;

    virtual void apply(const vector <string>& arguments) = 0;
};

/**
 * @brief Parser for the Thresholding transformation.
 * @see transforms::Thresholding
 */
class ThresholdingParser: public Parser<MatrixXi, MatrixXi> {
public:
    /**
     * @brief Construct a new Thresholding Parser object
     * 
     */
    ThresholdingParser();

    /**
     * @brief Instantiate a threshold transform.
     * 
     * @param arguments List of arguments (parameters of the transform) passed through the command line.
     * @return Transform<MatrixXi, MatrixXi>* Instance of the Thresholding transform.
     */
    Transform<MatrixXi, MatrixXi>* parse(const vector<string>& arguments) override;

    /**
     * @brief Apply the Thresholding transform (read the input file, create and use the transform, save the output file).
     * 
     * @param arguments List of arguments (parameters of the transform) passed through the command line.
     */
    void apply(const vector <string>& arguments) override;
};

class HistogramParser: public Parser<MatrixXi, MatrixXd> {
public:
    HistogramParser();
    Transform<MatrixXi, MatrixXd>* parse(const vector<string>& arguments) override;
    void apply(const vector <string>& arguments) override;
};

class FFT2DFreqParser: public Parser<MatrixXi, Eigen::Matrix<std::complex<double>,-1, -1>>{
public:
    FFT2DFreqParser();
    Transform<MatrixXi, Eigen::Matrix<std::complex<double>,-1, -1>>* parse(const vector<string>& arguments) override;
    void apply(const vector <string>& arguments) override;
};

class FFT2DMagParser: public Parser<MatrixXi, Eigen::Matrix<std::complex<double>,-1, -1>>{
public:
    FFT2DMagParser();
    FFT2D* parse(const vector<string>& arguments) override;
    void apply(const vector <string>& arguments) override;
};

class iFFT2DParser: public Parser<Eigen::Matrix<std::complex<double>,-1, -1>, MatrixXi>{
    iFFT2DParser();
    Transform<Eigen::Matrix<std::complex<double>,-1, -1>, MatrixXi>* parse(const vector<string>& arguments) override;
    void apply(const vector <string>& arguments) override;
};

class HighpassFilterParser: public Parser<MatrixXi, MatrixXi> {
public:
    HighpassFilterParser();
    Transform<MatrixXi, MatrixXi>* parse(const vector<string>& arguments) override;
    void apply(const vector <string>& arguments) override;
};

class LowpassFilterParser: public Parser<MatrixXi, MatrixXi> {
public:
    LowpassFilterParser();
    Transform<MatrixXi, MatrixXi>* parse(const vector<string>& arguments) override;
    void apply(const vector <string>& arguments) override;
};

#endif