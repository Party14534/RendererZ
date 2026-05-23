#include "math.h"

Mat::Mat(size_t rows, size_t cols) : rows(rows), cols(cols) {
    values = std::vector<float>(rows * cols);
}

Mat::Mat(size_t rows, size_t cols, const std::vector<float>& _values) :
    rows(rows), cols(cols)
{
    if (_values.size() != rows * cols) { 
        std::cout << "Invalid vector size\n";
        exit(1); 
    }

    values = std::vector<float>(rows * cols);
    std::copy(_values.begin(), _values.end(), values.begin());
}

float Mat::get(const size_t i, const size_t j) const {
    if (i >= rows || j >= cols) {
        std::cout << "Invalid indices\n";
        exit(1);
    }
    return values[i*cols + j];
}

void Mat::set(const size_t i, const size_t j, const float val) {
    if (i >= rows || j >= cols) {
        std::cout << "Invalid indices\n";
        exit(1);
    }
    values[i*cols + j] = val;
}

Mat Mat::operator+(const Mat& other) const {
    if (rows != other.rows || cols != other.cols) {
        std::cout << "Matrices cannot be added\n";
        exit(1);
    }

    std::vector<float> vals(values.size());

    for(size_t i = 0; i < values.size(); i++) {
        vals[i] = values[i] + other.values[i];
    }

    return Mat(rows, cols, vals);
}

Mat Mat::operator-(const Mat& other) const {
    if (rows != other.rows || cols != other.cols) {
        std::cout << "Matrices cannot be added\n";
        exit(1);
    }

    std::vector<float> vals(values.size());

    for(size_t i = 0; i < values.size(); i++) {
        vals[i] = values[i] - other.values[i];
    }

    return Mat(rows, cols, vals);
}

Mat Mat::operator*(const Mat& other) const {
    if (cols != other.rows) {
        std::cout << "Matrices cannot be multiplied\n";
        exit(1);
    }

    std::vector<float> vals(rows * other.cols, 0.f);

    Mat otherT = other.transpose();

    const float* a = values.data();
    const float* b = otherT.values.data();

    #pragma omp parallel for
    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < other.cols; c++) {
            float sum = 0.f;
            
            #pragma omp simd reduction(+:sum)
            for (size_t k = 0; k < cols; k++) {
                sum += a[r*cols+k] * b[c*cols+k];
            }

            vals[r*other.cols+c] = sum;
        }
    }

    return Mat(rows, other.cols, vals);
}

Vec4 Mat::operator*(const Vec4& vec) const {
    if (cols != 4 || rows != 4) {
        std::cout << "Matrix cannot be multiplied with Vec4\n";
        exit(1);
    }

    Vec4 output;

    for (int i = 0; i < 4; i++) {
        float sum = 0.f;

        #pragma omp simd reduction(+:sum)
        for (int j = 0; j < 4; j++) {
            sum += values[i*cols+j] * (&vec.x)[j];
        }

        (&output.x)[i] = sum;
    }
    
    return output;
}

Mat Mat::transpose() const {
    std::vector<float> vals(values.size());

    #pragma omp parallel for
    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < cols; c++) {
            vals[c * rows + r] = values[r * cols + c];
        }
    }

    return Mat(cols, rows, vals);
}

/*
 * Helper functions
 */

std::ostream& operator <<(std::ostream& os, const Mat& m) {
    return os << m.toString();
}

std::string Mat::toString() const {
    std::string s = "[";
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            s += std::to_string(get(i, j));
            if (i * cols + j < values.size() - 1) { s += ", "; }
        }
        if (i != rows - 1) s += "\n";
    }

    return s + "]\n";
}

static std::mt19937 generator(std::random_device{}());
static std::uniform_real_distribution<float> distribution(0.f, 100000.f);

Mat generateRandomMatrix(size_t rows, size_t cols) {
    std::vector<float> vals(rows * cols);

    for (auto& val : vals) {
        val = distribution(generator);
    }

    return Mat(rows, cols, vals);
}

Mat Mat::getIdentity(const size_t s) {
    Mat m = Mat(s, s);

    for (int i = 0; i < s; i++) {
        m.set(i, i, 1.f);
    }

    return m;
}
