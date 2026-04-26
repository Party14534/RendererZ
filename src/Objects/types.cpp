#include "Types.h"

Vec3::Vec3() : x(0.), y(0.), z(0.) {}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

Vec3 Vec3::operator+(const Vec3 v) const {
    return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator-(const Vec3 v) const {
    return Vec3(x - v.x, y - v.y, z - v.z);
}

float Vec3::length() const {
    return sqrt(x*x + y*y + z*z);
}

Vec3 Vec3::normalize() const {
    float len = length();
    return {x / len, y / len, z / len};
}

float Vec3::dot(Vec3 vec) const {
    return x*vec.x + y*vec.y + z*vec.z;
}

Vec3 Vec3::cross(Vec3 vec) const {
    return {
        y*vec.z - z*vec.y,
        z*vec.x - x*vec.z,
        x*vec.y - y*vec.x
    };
}

Vec4::Vec4() : x(0.), y(0.), z(0.) {}

Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Vec4 Vec4::operator+(const Vec4 v) const {
    return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vec4 Vec4::operator-(const Vec4 v) const {
    return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

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

std::ostream& operator <<(std::ostream& os, const Mat4& m) {
    return os << "\n[\n" << m.data[0] << ", " << m.data[1] << ", " << m.data[2] << ", " << m.data[3] << "\n"
        << m.data[4] << ", " << m.data[5] << ", " << m.data[6] << ", " << m.data[7] << "\n"
        << m.data[8] << ", " << m.data[9] << ", " << m.data[10] << ", " << m.data[11] << "\n"
        << m.data[12] << ", " << m.data[13] << ", " << m.data[14] << ", " << m.data[15] << "\n]\n";
};

float Mat4::get(u32 i, u32 j) const {
    return data[i * 4 + j];
}

/*
 * Color
 */
Color::Color() : r(0.), g(0.), b(0.), a(0.) {}
Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
