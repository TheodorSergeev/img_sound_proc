#include <iostream>
#include <Eigen/Dense>
//#include "opencv2/core.hpp"

using std::cout;
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
                cout << item(i, j) - min_val  << " " << size << "\n";
                hist(0,item(i, j) - min_val) += 1. / size;
            }
        }

        return hist;
    };
};

a

int main() {
    //Matrix<int, -1, 1> img;
    MatrixXi img(3,3);
    img << 1, 2, 3,
           4, 5, 6,
           7, 8, 9;

    cout << "Original\n" << img << "\n";

    Thresholding threshold(3,6);
    cout << "Thresholding\n" << threshold.transform(img) << "\n";

    Histogram hist;
    cout << "Histogram\n" << hist.transform(img);

    return 0;
}
