#include "utils.hpp"
#include <complex>


/* Readers/writers for Eigen matrices */

MatrixXi readIntMatrix(const string& inp_fname) {
    string extension(std::experimental::filesystem::path(inp_fname).extension());

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

void writeDoubleMatrix(const string& out_fname, const MatrixXd& matrix) {

    std::ofstream write_output(out_fname);
    assert(write_output.is_open());
    write_output << matrix;
    write_output.close();
    cout << "File saved: " << out_fname << "\n";
    //Under what circumstances will write fail?
}

void writeComplexMatrix(const string& out_fname, const Eigen::Matrix<std::complex<double>,-1, -1>& matrix) {

    std::ofstream write_output(out_fname);
    assert(write_output.is_open());
    write_output << matrix;
    write_output.close();
    cout << "File saved: " << out_fname << "\n";
    //Under what circumstances will write fail?
}

MatrixXd readFloatMatrix(const string& inp_fname) {
    string extension(std::experimental::filesystem::path(inp_fname).extension());

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



/* Conversion of matrices between internal types */

MatrixXi opencv2eigen(const cv::Mat& image) {
    int n_cols = image.cols;
    int n_rows = image.rows;
    MatrixXi mat(n_rows, n_cols);

    for (int i = 0; i < n_cols; ++i) {
        for (int j = 0; j < n_rows; ++j) {
            mat(j, i) = (int) image.at<char>(j, i);
        }
    }

    return mat;
}

cv::Mat eigen2opencv(const MatrixXi& mat) {
    int n_cols = mat.cols();
    int n_rows = mat.rows();
    cv::Mat image = cv::Mat::zeros(n_rows, n_cols, CV_8U);

    for (int i = 0; i < n_cols; ++i) {
        for (int j = 0; j < n_rows; ++j) {
            image.at<char>(j, i) = mat(j, i);
        }
    }

    return image;
}
