#include "parsing.hpp"


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


/* Parser */

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

template <typename TInput, typename TOutput>
Transform<TInput, TOutput>* Parser<TInput, TOutput>::parse(const vector<string>& arguments) = 0;


/* ThresholdingParser*/

ThresholdingParser::ThresholdingParser() {
    arg_num = 2;
    name = "threshold";
}

Transform<MatrixXi, MatrixXi>* ThresholdingParser::parse(const vector<string>& arguments) {
    cout << "n_args = " << arguments.size() << "\n";

    if (arguments.size() != 2 + arg_num)
        throw std::invalid_argument("Thresholding requires exactly two integer arguments.");
                
    // throw an exception if not convertible to int
    int thr_min = std::stoi(arguments[2]);
    int thr_max = std::stoi(arguments[3]);

    return new Thresholding(thr_min, thr_max);
}

void ThresholdingParser::apply(const vector <string>& arguments) {
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
