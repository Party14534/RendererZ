#include "math.h"

/*
 * Mat
 */

Mat::Mat(size_t rows, size_t cols) : rows(rows), cols(cols) {
    values = std::vector<float>(rows * cols);
}

Mat::Mat(size_t rows, size_t cols, const float _value) :
    rows(rows), cols(cols)
{
    values = std::vector<float>(rows * cols, _value);
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

Mat::Mat(const MatD& other) : rows(other.rows), cols(other.cols) {
    values = std::vector<float>(other.values.size());
    for (size_t i = 0; i < other.values.size(); ++i) {
        values[i] = static_cast<float>(other.values[i]);
    }
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

Mat Mat::scaleDown() const {
    Mat m(rows - 1, cols - 1);
    
    for (u32 i = 0; i < rows - 1; i++) {
        for (u32 j = 0; j < cols - 1; j++) {
            m.set(i, j, get(i, j));
        }
    } 

    return m;
}

Mat Mat::scaleUp() const {
    Mat m(rows + 1, cols + 1);
    
    for (u32 i = 0; i < rows; i++) {
        for (u32 j = 0; j < cols; j++) {
            m.set(i, j, get(i, j));
        }
    } 

    return m;
}

/*
 * MatD
 */

MatD::MatD(size_t rows, size_t cols) : rows(rows), cols(cols) {
    values = std::vector<double>(rows * cols);
}

MatD::MatD(size_t rows, size_t cols, const double _value) :
    rows(rows), cols(cols)
{
    values = std::vector<double>(rows * cols, _value);
}

MatD::MatD(size_t rows, size_t cols, const std::vector<double>& _values) :
    rows(rows), cols(cols)
{
    if (_values.size() != rows * cols) { 
        std::cout << "Invalid vector size\n";
        exit(1); 
    }

    values = std::vector<double>(rows * cols);
    std::copy(_values.begin(), _values.end(), values.begin());
}

double MatD::get(const size_t i, const size_t j) const {
    if (i >= rows || j >= cols) {
        std::cout << "Invalid indices\n";
        exit(1);
    }
    return values[i*cols + j];
}

void MatD::set(const size_t i, const size_t j, const double val) {
    if (i >= rows || j >= cols) {
        std::cout << "Invalid indices\n";
        exit(1);
    }
    values[i*cols + j] = val;
}

MatD MatD::operator+(const MatD& other) const {
    if (rows != other.rows || cols != other.cols) {
        std::cout << "Matrices cannot be added\n";
        exit(1);
    }

    std::vector<double> vals(values.size());

    for(size_t i = 0; i < values.size(); i++) {
        vals[i] = values[i] + other.values[i];
    }

    return MatD(rows, cols, vals);
}

MatD MatD::operator-(const MatD& other) const {
    if (rows != other.rows || cols != other.cols) {
        std::cout << "Matrices cannot be added\n";
        exit(1);
    }

    std::vector<double> vals(values.size());

    for(size_t i = 0; i < values.size(); i++) {
        vals[i] = values[i] - other.values[i];
    }

    return MatD(rows, cols, vals);
}

MatD MatD::operator*(const MatD& other) const {
    if (cols != other.rows) {
        std::cout << "Matrices cannot be multiplied\n";
        exit(1);
    }

    std::vector<double> vals(rows * other.cols, 0.f);

    MatD otherT = other.transpose();

    const double* a = values.data();
    const double* b = otherT.values.data();

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

    return MatD(rows, other.cols, vals);
}

Vec4 MatD::operator*(const Vec4& vec) const {
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

MatD MatD::transpose() const {
    std::vector<double> vals(values.size());

    #pragma omp parallel for
    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < cols; c++) {
            vals[c * rows + r] = values[r * cols + c];
        }
    }

    return MatD(cols, rows, vals);
}

MatD MatD::scaleDown() const {
    MatD m(rows - 1, cols - 1);
    
    for (u32 i = 0; i < rows - 1; i++) {
        for (u32 j = 0; j < cols - 1; j++) {
            m.set(i, j, get(i, j));
        }
    } 

    return m;
}

MatD MatD::scaleUp() const {
    MatD m(rows + 1, cols + 1);
    
    for (u32 i = 0; i < rows; i++) {
        for (u32 j = 0; j < cols; j++) {
            m.set(i, j, get(i, j));
        }
    } 

    return m;
}

Mat MatD::toMat() const {
    Mat m(cols, rows);
    for (size_t i = 0; i < values.size(); ++i) {
        m.values[i] = static_cast<float>(values[i]);
    }

    return m;
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

MatD MatD::getIdentity(const size_t s) {
    MatD m = MatD(s, s);

    for (int i = 0; i < s; i++) {
        m.set(i, i, 1.);
    }

    return m;
}

/*
 * Mat4
 */

Mat4::Mat4(std::initializer_list<float> _values) {
    std::copy(_values.begin(), _values.end(), values);
}

Mat4 Mat4::operator*(const Mat4& other) const {
    Mat4 result;

    for (size_t r = 0; r < 4; r++) {
        for (size_t c = 0; c < 4; c++) {
            float sum = 0.f;
            for (size_t k = 0; k < 4; k++) {
                sum += values[r*4+k] * other.values[k*4+c];
            }
            result.values[r*4+c] = sum;
        }
    }

    return result;
}

Vec4 Mat4::operator*(const Vec4& vec) const {
    Vec4 output;

    for (int i = 0; i < 4; i++) {
        float sum = 0.f;
        for (int j = 0; j < 4; j++) {
            sum += values[i*4+j] * (&vec.x)[j];
        }
        (&output.x)[i] = sum;
    }

    return output;
}

/*
 * Mat4D
 */

Mat4D::Mat4D(std::initializer_list<double> _values) {
    std::copy(_values.begin(), _values.end(), values);
}

Mat4D Mat4D::operator*(const Mat4D& other) const {
    Mat4D result;

    for (size_t r = 0; r < 4; r++) {
        for (size_t c = 0; c < 4; c++) {
            double sum = 0.;
            for (size_t k = 0; k < 4; k++) {
                sum += values[r*4+k] * other.values[k*4+c];
            }
            result.values[r*4+c] = sum;
        }
    }

    return result;
}

Vec4 Mat4D::operator*(const Vec4& vec) const {
    Vec4 output;

    for (int i = 0; i < 4; i++) {
        float sum = 0.f;
        for (int j = 0; j < 4; j++) {
            sum += values[i*4+j] * (&vec.x)[j];
        }
        (&output.x)[i] = sum;
    }

    return output;
}

MatD Mat4D::toMatD() const {
    return MatD(4, 4, std::vector<double>(values, values + 16));
}

Mat4D lookAt(Vec3 eye, Vec3 target, Vec3 up) {
    Vec3 direction = (eye - target).normalize();
    Vec3 right = up.cross(direction).normalize();
    Vec3 camUp = direction.cross(right);

    Mat4D m1({
        right.x, right.y, right.z, 0,
        camUp.x, camUp.y, camUp.z, 0,
        direction.x, direction.y, direction.z, 0,
        0, 0, 0, 1
    });

    Mat4D m2({
        1, 0, 0, -eye.x,
        0, 1, 0, -eye.y,
        0, 0, 1, -eye.z,
        0, 0, 0, 1
    });

    return m1 * m2;
}
