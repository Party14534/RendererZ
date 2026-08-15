#ifndef MAT_H
#define MAT_H

#include "vec.h"
#include <string>
#include <type_traits>
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <random>

struct MatD;

struct Mat {
    std::vector<float> values;
    size_t rows, cols;

    Mat(size_t rows, size_t cols);
    Mat(size_t rows, size_t cols, const float _value);
    Mat(size_t rows, size_t cols, const std::vector<float>& values);
    Mat(const MatD& other); // implicit conversion: each double truncated to float

    float get(const size_t i, const size_t j) const;
    void set(const size_t i, const size_t j, const float val);

    std::string toString() const;

    Mat operator+(const Mat& other) const;
    Mat operator-(const Mat& other) const;
    Mat operator*(const Mat& other) const;
    Vec4 operator*(const Vec4& other) const;

    Mat transpose() const;
    Mat scaleDown() const;
    Mat scaleUp() const;
    
    static Mat getIdentity(size_t s);

    friend std::ostream& operator <<(std::ostream& os, const Mat& m);
};

struct MatD {
    std::vector<double> values;
    size_t rows, cols;

    MatD(size_t rows, size_t cols);
    MatD(size_t rows, size_t cols, const double _value);
    MatD(size_t rows, size_t cols, const std::vector<double>& values);

    double get(const size_t i, const size_t j) const;
    void set(const size_t i, const size_t j, const double val);

    std::string toString() const;

    MatD operator+(const MatD& other) const;
    MatD operator-(const MatD& other) const;
    MatD operator*(const MatD& other) const;
    Vec4 operator*(const Vec4& other) const;

    MatD transpose() const;
    MatD scaleDown() const;
    MatD scaleUp() const;
    
    static MatD getIdentity(size_t s);

    Mat toMat() const;

    friend std::ostream& operator <<(std::ostream& os, const MatD& m);
};


// Non-member function
Mat generateRandomMatrix(size_t rows, size_t cols);
MatD generateRandomMatrixD(size_t rows, size_t cols);

#endif
