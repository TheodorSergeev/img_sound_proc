#include "transforms.hpp"

// Transform

// doesn't work for some reason
// template <typename TInput, typename TOutput>
// TOutput Transform<TInput, TOutput>::transform(const TInput& item_) = 0;

// Thresholding

Thresholding::Thresholding(const int thr_min_, const int thr_max_) {
    thr_min = thr_min_;
    thr_max = thr_max_;

    if (thr_min > thr_max) {
        throw std::invalid_argument(
            "Minimum value of the threshold cannot be bigger then the maximum");
    }
}

MatrixXi Thresholding::transform(const MatrixXi &item) {
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
}

// Histogram

Histogram::Histogram() = default;

MatrixXd Histogram::transform(const MatrixXi &item) {
    int nrows = item.rows();
    int ncols = item.cols();
    int size = nrows * ncols;

    int min_val = item.minCoeff();
    int max_val = item.maxCoeff();

    MatrixXd hist = MatrixXd::Zero(1, max_val - min_val + 1);

    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            hist(0, item(i, j) - min_val) += 1. / size;
        }
    }

    return hist;
}

// Fourier Transform

/* define mainbody of FFT1D */
void mainFFT1D(std::complex<float> signal[], int start, int fin, int step1,
               float inv, std::complex<float> buffer[]) {
    int n = (fin - start) / step1 + 1;
    if (n == 1) {  // Termination condition: only one element left in the
                   // considered array
        return;
    }
    mainFFT1D(signal, start, fin, 2 * step1, inv,
              buffer);  // recursively call on even indices
    mainFFT1D(signal, start + step1, fin, 2 * step1, inv,
              buffer);  // recursively call on odd indices

    /*building factor*/
    std::complex<float> subfactor =
        std::polar(float(1), float(inv * 2 * M_PI / float(n)));
    std::complex<float> factor = 1;

    for (int k = 0; k < n; k++) {
        buffer[k] = signal[start + k * step1];
    }
    for (int k = 0; k < n / 2; k++) {
        signal[start + k * step1] = buffer[2 * k] + factor * buffer[2 * k + 1];
        signal[start + (k + n / 2) * step1] =
            buffer[2 * k] - factor * buffer[2 * k + 1];
        factor *= subfactor;
    }
}

FFT1D::FFT1D(int n) {
    step = n;
    transformed = 0;
}

void normalize(std::complex<float> f[], int n, float norm) {
    for (int i = 0; i < n; i++) {
        f[i].real(float(f[i].real() / norm));
        f[i].imag(float(f[i].imag() / norm));
    }
}

Eigen::Matrix<std::complex<double>, 1, Dynamic> FFT1D::getMagnitude() {
    if (transformed == 0) {
        throw std::logic_error("perform transform first");
    }
    return mMagnitude;
}

Eigen::Matrix<std::complex<double>, 1, Dynamic> FFT1D::transform(const MatrixXi &item) {
    int nrows = item.rows();
    int ncols = item.cols();
    int size = nrows * ncols;

    /*convert image matrix to a complex array*/
    std::complex<float> *spatial = new std::complex<float>[size];
    std::complex<float> a;
    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            a.real(item(i, j));
            a.imag(0);
            spatial[i * ncols + j] = a;
        }
    }
    std::complex<float> *buffer = new std::complex<float>[size];
    for (int i = 0; i < size; i++) {
        buffer[i] = 0;
    }
    mainFFT1D(spatial, 0, size - 1, step, float(-1), buffer);
    normalize(spatial, size, float(std::sqrt(size)));
    delete[] buffer;

    mfrequencyDomain.resize(1, size);
    mMagnitude.resize(1, size);

    for (int i = 0; i < size; i++) {
        mfrequencyDomain[0, i] = spatial[i];
    }
    for (int i = 0; i < size; i++) {
        mMagnitude[0, i] = std::abs(spatial[i]);
    }
    delete[] spatial;
    transformed = 1;

    return mfrequencyDomain;
}

Eigen::Matrix<int, 1, Dynamic> iFFT1D::transform(const Eigen::Matrix<std::complex<double>, 1, -1> &item) {
    int nrows = item.rows();
    int ncols = item.cols();
    int size = nrows * ncols;

    /*convert image matrix to a complex array*/
    std::complex<float> *frequency = new std::complex<float>[size];
    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            frequency[i * ncols + j] = item[i, j];
        }
    }
    std::complex<float> *buffer = new std::complex<float>[size];
    for (int i = 0; i < size; i++) {
        buffer[i] = 0;
    }
    mainFFT1D(frequency, 0, size - 1, 1, float(1), buffer);
    normalize(frequency, size, float(std::sqrt(size)));
    delete[] buffer;

    mspatialDomain.resize(1, size);
    for (int i = 0; i < size; i++) {
        mspatialDomain[0, i] = round(frequency[i].real());
    }
    delete[] frequency;
    transformed = 1;
    std::cout << mspatialDomain;
    return mspatialDomain;
}

FFT2D::FFT2D(int n) {
    step = n;
    transformed = 0;
}

Eigen::Matrix<std::complex<double>, -1, -1> FFT2D::getMagnitude() {
    if (transformed == 0) {
        throw std::logic_error("perform transform first");
    }
    return mMagnitude;
}

Eigen::Matrix<std::complex<double>, -1, -1> FFT2D::transform(const MatrixXi &item) {
    int nrows = item.rows();
    int ncols = item.cols();
    int size = nrows * ncols;

    std::complex<float> *spatial = new std::complex<float>[size];
    std::complex<float> a;
    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            a.real(item(i, j));
            a.imag(0);
            spatial[i * ncols + j] = a;
        }
    }
    std::complex<float> *buffer;
    for (int i = 0; i < nrows; ++i) {
        buffer = new std::complex<float>[ncols];
        mainFFT1D(spatial, i * ncols, (i + 1) * ncols - 1, step, -1, buffer);
        delete[] buffer;
    }
    for (int j = 0; j < ncols; ++j) {
        buffer = new std::complex<float>[nrows];
        mainFFT1D(spatial, j, ncols * (nrows - 1) + j, ncols, -1, buffer);
        delete[] buffer;
    }
    normalize(spatial, size, float(std::sqrt(size)));

    mfrequencyDomain.resize(nrows, ncols);
    mMagnitude.resize(nrows, ncols);

    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            mfrequencyDomain(i, j) = spatial[i * ncols + j];
        }
    }
    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            mMagnitude(i, j) = std::abs(spatial[i * ncols + j]);
        }
    }

    delete[] spatial;
    transformed = 1;

    return mfrequencyDomain;
}

Eigen::Matrix<int, -1, -1> iFFT2D::transform(const Eigen::Matrix<std::complex<double>, -1, -1> &item) {
    int nrows = item.rows();
    int ncols = item.cols();
    int size = nrows * ncols;

    /*convert image matrix to a complex array*/
    std::complex<float> *frequency = new std::complex<float>[size];
    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            frequency[i * ncols + j] = item(i, j);
        }
    }

    std::complex<float> *buffer;
    for (int i = 0; i < nrows; ++i) {
        buffer = new std::complex<float>[ncols];
        mainFFT1D(frequency, i * ncols, (i + 1) * ncols - 1, 1, 1, buffer);
        delete[] buffer;
    }
    for (int j = 0; j < ncols; ++j) {
        buffer = new std::complex<float>[nrows];
        mainFFT1D(frequency, j, ncols * (nrows - 1) + j, ncols, 1, buffer);
        delete[] buffer;
    }
    normalize(frequency, size, float(std::sqrt(size)));
    mspatialDomain.resize(nrows, ncols);
    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            mspatialDomain(i, j) = round(frequency[i * ncols + j].real());
        }
    }
    delete[] frequency;
    transformed = 1;

    return mspatialDomain;
}

LowpassFilter::LowpassFilter(const double threshold, int step) {
    thr = threshold;
    stp = step;
    transformed = 0;
}

MatrixXi LowpassFilter::transform(const MatrixXi &item) {
    int nrows = item.rows();
    int ncols = item.cols();
    int size = nrows * ncols;
    int rowc = nrows / 2;
    int colc = ncols / 2;

    /*perform 2DFFT*/
    FFT2D a(stp);
    Eigen::Matrix<std::complex<double>, -1, -1> copyfrequency;
    copyfrequency.resize(nrows, ncols);
    copyfrequency = a.transform(item);

    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            if (double(std::sqrt((i - rowc) * (i - rowc) +
                                 (j - colc) * (j - colc))) > thr) {
                copyfrequency(i, j) = std::complex<double>(0.0, 0.0);
            } else {
                continue;
            }
        }
    }
    iFFT2D b;
    filtered.resize(nrows, ncols);
    filtered = b.transform(copyfrequency);
    transformed = 1;
    return filtered;
}

HighpassFilter::HighpassFilter(const double threshold, int step) {
    thr = threshold;
    stp = step;
    transformed = 0;
}

MatrixXi HighpassFilter::transform(const MatrixXi &item) {
    int nrows = item.rows();
    int ncols = item.cols();
    int size = nrows * ncols;
    int rowc = nrows / 2;
    int colc = ncols / 2;

    /*perform 2DFFT*/
    FFT2D a(stp);
    Eigen::Matrix<std::complex<double>, -1, -1> copyfrequency;
    copyfrequency.resize(nrows, ncols);
    copyfrequency = a.transform(item);

    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            if (double(std::sqrt((i - rowc) * (i - rowc) +
                                 (j - colc) * (j - colc))) <= thr) {
                copyfrequency(i, j) = std::complex<double>(0.0, 0.0);
            } else {
                continue;
            }
        }
    }
    iFFT2D b;
    filtered.resize(nrows, ncols);
    filtered = b.transform(copyfrequency);
    transformed = 1;
    return filtered;
}