#include "../Types.h"

/*
 * Matrix
 */

Mat4::Mat4() {}

Mat4::Mat4(const std::array<float, 16> _data) : data(_data)  { }

float& Mat4::operator[](u32 i) {
    return data[i];
}

Mat4 Mat4::operator+(const float s) const {
    Mat4 c;

    for (u32 i = 0; i < 16; i++) {
        c[i] = data[i] + s; 
    }

    return c;
}

Mat4 Mat4::operator-(const float s) const {
    Mat4 c;

    for (u32 i = 0; i < 16; i++) {
        c[i] = data[i] - s; 
    }

    return c;
}

Mat4 Mat4::operator*(const float s) const {
    Mat4 c;

    for (u32 i = 0; i < 16; i++) {
        c[i] = data[i] * s; 
    }

    return c;
}

Mat4 Mat4::operator/(const float s) const {
    Mat4 c;

    for (u32 i = 0; i < 16; i++) {
        c[i] = data[i] / s; 
    }

    return c;
}

Mat4 Mat4::operator+(const Mat4 m) const {
    Mat4 o;

    for (int i = 0; i < 16; i++) {
        o[i] = data[i] + m.data[i];
    }

    return o;
}

Mat4 Mat4::operator-(const Mat4 m) const {
    Mat4 o;

    for (int i = 0; i < 16; i++) {
        o[i] = data[i] - m.data[i];
    }

    return o;
}

Mat4 Mat4::operator*(const Mat4 m) const {
    Mat4 o;

    for (u32 i = 0; i < 4; i++) {
        for (u32 j = 0; j < 4; j++) {
            o[i*4 + j] = get(i, 0) * m.get(0, j) +
                get(i, 1) * m.get(1, j) +
                get(i, 2) * m.get(2, j) + 
                get(i, 3) * m.get(3, j);
        }
    }

    return o;
}

Vec4 Mat4::operator*(const Vec4 v) const {
    Vec4 o;

    o.x = get(0, 0) * v.x +
        get(0, 1) * v.y +
        get(0, 2) * v.z + 
        get(0, 3) * v.w;

    o.y = get(1, 0) * v.x +
        get(1, 1) * v.y +
        get(1, 2) * v.z + 
        get(1, 3) * v.w;

    o.z = get(2, 0) * v.x +
        get(2, 1) * v.y +
        get(2, 2) * v.z + 
        get(2, 3) * v.w;

    o.w = get(3, 0) * v.x +
        get(3, 1) * v.y +
        get(3, 2) * v.z + 
        get(3, 3) * v.w;

    return o;
}

std::ostream& operator <<(std::ostream& os, const Mat4& m) {
    return os << "\n[\n" << m.data[0] << ", " << m.data[1] << ", " << m.data[2] << ", " << m.data[3] << "\n"
        << m.data[4] << ", " << m.data[5] << ", " << m.data[6] << ", " << m.data[7] << "\n"
        << m.data[8] << ", " << m.data[9] << ", " << m.data[10] << ", " << m.data[11] << "\n"
        << m.data[12] << ", " << m.data[13] << ", " << m.data[14] << ", " << m.data[15] << "\n]\n";
};

float Mat4::get(u32 i, u32 j) const {
    return data[i * 4 + j];
}
