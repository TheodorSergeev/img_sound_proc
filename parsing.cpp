#include "parsing.hpp"


void parse_cl_input(int argc, const char* argv[]) {
    const char* HELP_STR = "--help";
    const char* HELP_MSG = "asdasd";
    std::array parsers_list = {ThresholdingParser()}; // todo: pass as an argument?
    // todo: add parser list

    string opt_str(argv[1]);

    if (argc == 1) {
        cout << "Not enough arguments. Use \"./img_sound-proc --help\" to access the options.\n";
    } else {
        string opt_str(argv[1]);

        cout << opt_str << "\n";

        if (opt_str == HELP_STR) {
            cout << HELP_MSG << "\n";
        } else {
            for (auto  &parser : parsers_list) {
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


// Parser

template <typename TInput, typename TOutput>
void Parser<TInput, TOutput>::checkArgNum(const vector<string>& arguments) {
    if (arguments.size() != 2 + get_arg_num()) {
        throw std::invalid_argument(
            "Incorect number of parameters for " + get_name() + \
            "\nRequired: " + to_string(2 + get_arg_num()) + \
            "\nProvided: " + to_string(arguments.size() - 2) + "\n"
        );
    }
}

template <typename TInput, typename TOutput>
string Parser<TInput, TOutput>::get_name() {
    return name;
}

template <typename TInput, typename TOutput>
int Parser<TInput, TOutput>::get_arg_num() {
    return arg_num;
}

/*template <typename TInput, typename TOutput>
Transform<TInput, TOutput>* Parser<TInput, TOutput>::parse(const vector<string>& arguments) = 0;*/


// ThresholdingParser

ThresholdingParser::ThresholdingParser() {
    arg_num = 2;
    name = "threshold";
}

Transform<MatrixXi, MatrixXi>* ThresholdingParser::parse(const vector<string>& arguments) {
    cout << "n_args = " << arguments.size() << "\n";

    if (arguments.size() != 2 + arg_num)
        throw std::invalid_argument("Thresholding requires exactly two integer arguments.");
                
    // throws an exception if not convertible to int
    int thr_min = std::stoi(arguments[2]);
    int thr_max = std::stoi(arguments[3]);

    return new Thresholding(thr_min, thr_max);
}

void ThresholdingParser::apply(const vector <string>& arguments) {
    checkArgNum(arguments); 

    // todo: why opencv doesn't want to read from the relative path?
    string glob_path = std::experimental::filesystem::current_path();
    string inp_fname = glob_path + arguments[0];
    string out_fname = glob_path + arguments[1];

    MatrixXi input = readIntMatrix(inp_fname);
    auto thresh = parse(arguments);
    MatrixXi output = thresh->transform(input);
    writeIntMatrix(out_fname, output);
}

HistogramParser::HistogramParser() {
    arg_num = 0;
    name = "histogram";
}

Transform<MatrixXi, MatrixXd>* HistogramParser::parse(const vector<string>& arguments) {
    cout << "n_args = " << arguments.size() << "\n";

    if (arguments.size() != 2 + arg_num)
        throw std::invalid_argument("Thresholding requires no arguments.");
    return new Histogram();
}
void HistogramParser::apply(const vector<string> &arguments) {
    checkArgNum(arguments);
    string glob_path = std::experimental::filesystem::current_path();
    string inp_fname = glob_path + arguments[0];
    string out_fname = glob_path + arguments[1];
    MatrixXi input = readIntMatrix(inp_fname);
    auto thresh = parse(arguments);
    MatrixXd output = thresh ->transform(input);
    writeDoubleMatrix(out_fname, output);
}

FFT2DFreqParser::FFT2DFreqParser() {
    arg_num = 1;
    name = "fft2Dfreq";
}

Transform<MatrixXi, Eigen::Matrix<std::complex<double>,-1, -1>>* FFT2DFreqParser::parse(const vector<string>& arguments){
    cout << "n_args = " << arguments.size() << "\n";
    if (arguments.size() == 2 + arg_num){
        int fftstep = std::stoi(arguments[2]);
        return new FFT2D(fftstep);
    }
    else if (arguments.size() == 2){
        cout << "FFT step set to default: 1"<<std::endl;
        return new FFT2D();

    }
    else
        throw std::invalid_argument("Thresholding requires no arguments.");
}

void FFT2DFreqParser::apply(const vector<string> &arguments) {
    if (arguments.size() != 2 + get_arg_num() and arguments.size() != 2) {
        throw std::invalid_argument(
                "Incorect number of parameters for " + get_name() + \
            "\nRequired: 0 or 1"  + \
            "\nProvided: " + to_string(arguments.size() - 2) + "\n"
        );
    }
    string glob_path = std::experimental::filesystem::current_path();
    string inp_fname = glob_path + arguments[0];
    string out_fname = glob_path + arguments[1];
    MatrixXi input = readIntMatrix(inp_fname);
    auto thresh = parse(arguments);
    Eigen::Matrix<std::complex<double>,-1, -1> output = thresh ->transform(input);
    writeComplexMatrix(out_fname, output);
}

FFT2DMagParser::FFT2DMagParser() {
    arg_num = 1;
    name = "fft2Dmag";
}

FFT2D* FFT2DMagParser::parse(const vector<string>& arguments){
    cout << "n_args = " << arguments.size() << "\n";
    if (arguments.size() == 2 + arg_num){
        int fftstep = std::stoi(arguments[2]);
        return new FFT2D(fftstep);
    }
    else if (arguments.size() == 2){
        cout << "FFT step set to default: 1"<<std::endl;
        return new FFT2D();
    }
    else
        throw std::invalid_argument("Thresholding requires no arguments.");
}

void FFT2DMagParser::apply(const vector<string> &arguments) {
    if (arguments.size() != 2 + get_arg_num() and arguments.size() != 2) {
        throw std::invalid_argument(
                "Incorect number of parameters for " + get_name() + \
            "\nRequired: 0 or 1"  + \
            "\nProvided: " + to_string(arguments.size() - 2) + "\n"
        );
    }
    string glob_path = std::experimental::filesystem::current_path();
    string inp_fname = glob_path + arguments[0];
    string out_fname = glob_path + arguments[1];
    MatrixXi input = readIntMatrix(inp_fname);
    auto thresh = parse(arguments);
    thresh->transform(input);
    Eigen::Matrix<std::complex<double>,-1, -1> output = thresh ->getMagnitude();
    writeComplexMatrix(out_fname, output);
}

iFFT2DParser::iFFT2DParser() {
    arg_num = 0;
    name = "ifft2D";
}

Transform<Eigen::Matrix<std::complex<double>,-1, -1>, MatrixXi>* iFFT2DParser::parse(const vector<string>& arguments) {
    cout << "n_args = " << arguments.size() << "\n";

    if (arguments.size() != 2 + arg_num)
        throw std::invalid_argument("iFFT2D requires no arguments.");
    return new iFFT2D();
}

// after writing readComplexMatrix
/*void iFFT2DParser::apply(const vector<string> &arguments) {
    checkArgNum(arguments);
    string glob_path = std::experimental::filesystem::current_path();
    string inp_fname = glob_path + arguments[0];
    string out_fname = glob_path + arguments[1];
    Eigen::Matrix<std::complex<double>,-1, -1> input = readComplexMatrix(inp_fname);
    auto thresh = parse(arguments);
    MatrixXd output = thresh ->transform(input);
    writeIntMatrix(out_fname, output);
}*/

HighpassFilterParser::HighpassFilterParser(){
    arg_num = 1;
    name = "highpass";
}

Transform<MatrixXi, MatrixXi>* HighpassFilterParser::parse(const vector<string>& arguments) {
    cout << "n_args = " << arguments.size() << "\n";

    if (arguments.size() != 2 + arg_num)
        throw std::invalid_argument("Highpass filter requires one argument.");

    // throws an exception if not convertible to int
    int thr_high = std::stoi(arguments[2]);
    return new HighpassFilter(thr_high);
}

void HighpassFilterParser::apply(const vector <string>& arguments) {
    checkArgNum(arguments);

    string glob_path = std::experimental::filesystem::current_path();
    string inp_fname = glob_path + arguments[0];
    string out_fname = glob_path + arguments[1];

    MatrixXi input = readIntMatrix(inp_fname);
    auto thresh = parse(arguments);
    MatrixXi output = thresh->transform(input);
    writeIntMatrix(out_fname, output);
}

LowpassFilterParser::LowpassFilterParser(){
    arg_num = 1;
    name = "lowpass";
}

Transform<MatrixXi, MatrixXi>* LowpassFilterParser::parse(const vector<string>& arguments) {
    cout << "n_args = " << arguments.size() << "\n";

    if (arguments.size() != 2 + arg_num)
        throw std::invalid_argument("Lowpass filter requires one argument.");

    // throws an exception if not convertible to int
    int thr_high = std::stoi(arguments[2]);
    return new HighpassFilter(thr_high);
}

void LowpassFilterParser::apply(const vector <string>& arguments) {
    checkArgNum(arguments);

    string glob_path = std::experimental::filesystem::current_path();
    string inp_fname = glob_path + arguments[0];
    string out_fname = glob_path + arguments[1];

    MatrixXi input = readIntMatrix(inp_fname);
    auto thresh = parse(arguments);
    MatrixXi output = thresh->transform(input);
    writeIntMatrix(out_fname, output);
}