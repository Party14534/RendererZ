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

struct Mat {
    std::vector<float> values;
    size_t rows, cols;

    Mat(size_t rows, size_t cols);
    Mat(size_t rows, size_t cols, const std::vector<float>& values);

    float get(const size_t i, const size_t j) const;
    void set(const size_t i, const size_t j, const float val);

    std::string toString() const;

    Mat operator+(const Mat& other) const;
    Mat operator-(const Mat& other) const;
    Mat operator*(const Mat& other) const;
    Vec4 operator*(const Vec4& other) const;

    Mat transpose() const;

    friend std::ostream& operator <<(std::ostream& os, const Mat& m);
};

// Non-member function
Mat generateRandomMatrix(size_t rows, size_t cols);

#endif
