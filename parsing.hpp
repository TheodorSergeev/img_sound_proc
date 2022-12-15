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
 * @brief Abstract interface of parsers for transforms. 
 * Used for putting parsers in a list to iterate through them easily.
 * 
 */
class AbstractParser {
protected:
    int arg_num; /// Number of arguments that the transform requires.
    string name; /// Name of the transform in the command line.

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
     * @brief Apply the transform (e.g., parse argument, load image, do the transform, save the results).
     * 
     * @param arguments List of arguments (parameters of the transform) passed through the command line.
     */
    virtual void apply(const vector <string>& arguments) = 0;
};


/**
 * @brief Interface of parsers for transforms with input/output. 
 * The class describes how the command line arguments are treated,
 * how the transorm class is instantiated, and how input/output files are
 * processed.
 * 
 * @tparam TInput Type of the transform input (e.g., Eigen integer matrix).
 * @tparam TOutput Type of the transform output (e.g., Eigen float matrix).
 */
template <typename TInput, typename TOutput>
class Parser: public AbstractParser {
protected:
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
     * @brief Instantiate a corresponding transform with the specified parameters.
     * 
     * @param arguments List of arguments (parameters of the transform) passed through the command line.
     * @return Transform<TInput, TOutput>* Instance of the transform.
     */
    virtual Transform<TInput, TOutput>* parse(const vector<string>& arguments) = 0;
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


/**
 * @brief Parser for the histogram calculation.
 * @see transforms::Histogram
 */
class HistogramParser: public Parser<MatrixXi, MatrixXd> {
public:
    /**
     * @brief Construct a new Histogram Parser object
     *
     */
    HistogramParser();

    /**
     * @brief Instantiate a histogram calculation.
     *
     * @param arguments List of arguments (parameters of the transform) passed through the command line.
     * @return Transform<MatrixXi, MatrixXd>* Instance of the Histogram transform.
     */
    Transform<MatrixXi, MatrixXd>* parse(const vector<string>& arguments) override;

    /**
    * @brief Apply the Histogram transform (read the input file, create and use the transform, save the output file).
    *
    * @param arguments List of arguments (parameters of the transform) passed through the command line.
    */
    void apply(const vector <string>& arguments) override;
};


/**
 * @brief Parser for the FFT2D transform for frequency domain.
 * @see transforms::FFT2D
 */
class FFT2DFreqParser: public Parser<MatrixXi, Eigen::Matrix<std::complex<double>,-1, -1>>{
public:
    /**
     * @brief Construct a new FFT2D Parser object for frequency
     *
     */
    FFT2DFreqParser();

    /**
     * @brief Instantiate a FFT2D transform.
     *
     * @param arguments List of arguments (parameters of the transform) passed through the command line.
     * @return Transform<MatrixXi, Eigen::Matrix<std::complex<double>,-1, -1>>* Instance of the FFT2D transform for frequency domain.
     */
    Transform<MatrixXi, Eigen::Matrix<std::complex<double>,-1, -1>>* parse(const vector<string>& arguments) override;

    /**
    * @brief Apply the FFT2D transform for frequency domain(read the input file, create and use the transform to calculate frequency domain,
    * save the frequency domain to output file).
    *
    * @param arguments List of arguments (parameters of the transform) passed through the command line.
    */
    void apply(const vector <string>& arguments) override;
};


/**
 * @brief Parser for the FFT2D transform for magnitude.
 * @see transforms::FFT2D
 */
class FFT2DMagParser: public Parser<MatrixXi, Eigen::Matrix<std::complex<double>,-1, -1>>{
public:
    /**
    * @brief Construct a new FFT2D Parser object for magnitude
    *
    */
    FFT2DMagParser();

    /**
     * @brief Instantiate a FFT2D transform.
     *
     * @param arguments List of arguments (parameters of the transform) passed through the command line.
     * @return FFT2D* Instance of the FFT2D transform for magnitude.
     */
    FFT2D* parse(const vector<string>& arguments) override;

    /**
    * @brief Apply the FFT2D transform for magnitude (read the input file, create and use the transform to calculate magnitude,
    * save the magnitude to output file).
    *
    * @param arguments List of arguments (parameters of the transform) passed through the command line.
    */
    void apply(const vector <string>& arguments) override;
};


/**
 * @brief Parser for the iFFT2D transform.
 * @see transforms::iFFT2D
 */
class iFFT2DParser: public Parser<Eigen::Matrix<std::complex<double>,-1, -1>, MatrixXi>{
public:
    /**
    * @brief Construct a new iFFT2D Parser object
    *
    */
    iFFT2DParser();

    /**
     * @brief Instantiate an iFFT2D transform.
     *
     * @param arguments List of arguments (parameters of the transform) passed through the command line.
     * @return Transform<MatrixXi, Eigen::Matrix<std::complex<double>,-1, -1>>* Instance of the FFT2D transform for frequency domain.
     */
    Transform<Eigen::Matrix<std::complex<double>,-1, -1>, MatrixXi>* parse(const vector<string>& arguments) override;

    /**
    * @brief Apply the iFFT2D transform (read the input file, create and use the transform to calculate magnitude,
    * save the output file).
    *
    * @param arguments List of arguments (parameters of the transform) passed through the command line.
    */
    void apply(const vector <string>& arguments) override;
};


/**
 * @brief Parser for the highpass filter transform.
 * @see transforms::HighpassFilter
 */
class HighpassFilterParser: public Parser<MatrixXi, MatrixXi> {
public:
    /**
    * @brief Construct a new HighpassFilter Parser object
    *
    */
    HighpassFilterParser();

    /**
     * @brief Instantiate a highpass filter transform.
     *
     * @param arguments List of arguments (parameters of the transform) passed through the command line.
     * @return Transform<MatrixXi, MatrixXi>* Instance of the highpass filter transform.
     */
    Transform<MatrixXi, MatrixXi>* parse(const vector<string>& arguments) override;

    /**
    * @brief Apply the highpass filter transform (read the input file, create and use the transform,
    * save the output file).
    *
    * @param arguments List of arguments (parameters of the transform) passed through the command line.
    */
    void apply(const vector <string>& arguments) override;
};


/**
 * @brief Parser for the lowpass filter transform.
 * @see transforms::LowpassFilter
 */
class LowpassFilterParser: public Parser<MatrixXi, MatrixXi> {
public:
    /**
    * @brief Construct a new LowpassFilter Parser object
    *
    */
    LowpassFilterParser();

    /**
     * @brief Instantiate a lowpass filter transform.
     *
     * @param arguments List of arguments (parameters of the transform) passed through the command line.
     * @return Transform<MatrixXi, MatrixXi>* Instance of the lowpass filter transform.
     */
    Transform<MatrixXi, MatrixXi>* parse(const vector<string>& arguments) override;

    /**
    * @brief Apply the lowpass filter transform (read the input file, create and use the transform,
    * save the output file).
    *
    * @param arguments List of arguments (parameters of the transform) passed through the command line.
    */
    void apply(const vector <string>& arguments) override;
};

#endif