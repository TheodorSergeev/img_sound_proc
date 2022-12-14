#ifndef TRANSFORMS
#define TRANSFORMS

#include <vector>
#include <exception>
#include "Eigen/Dense"

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
    explicit Thresholding(const int thr_min_, const int thr_max_);

    MatrixXi transform(const MatrixXi& item) override;
};


class Histogram: public Transform<MatrixXi, MatrixXd> {
public:
    explicit Histogram();

    MatrixXd transform(const MatrixXi& item) override;
};

#endif