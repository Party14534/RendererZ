#include "math.h"

/*
 * Vec2
 */

Vec2::Vec2() : x(0), y(0) {}

Vec2::Vec2(float x, float y) : x(x), y(y) {}


Vec2 Vec2::operator+(const Vec2& other) const {
    return Vec2(x+other.x, y+other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const {
    return Vec2(x-other.x, y-other.y);
}

Vec2 Vec2::operator*(const float scalar) const {
    return Vec2(x*scalar, y*scalar);
}

/*
 * Vec3
 */

Vec3::Vec3() : x(0), y(0), z(0) {}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}


Vec3 Vec3::operator+(const Vec3& other) const {
    return Vec3(x+other.x, y+other.y, z+other.z);
}

Vec3 Vec3::operator-(const Vec3& other) const {
    return Vec3(x-other.x, y-other.y, z-other.z);
}

Vec3 Vec3::operator*(const float scalar) const {
    return Vec3(x*scalar, y*scalar, z*scalar);
}

float Vec3::length() const {
    return sqrt(x*x + y*y + z*z);
}

float Vec3::dot(const Vec3& other) const {
    return x*other.x + y*other.y + z*other.z;
}

Vec3 Vec3::normalize() const {
    float len = length();
    return Vec3(x / len, y / len, z / len);
}

Vec3 Vec3::cross(const Vec3& other) const {
    return Vec3(
        y*other.z - z*other.y,
        z*other.x - x*other.z,
        x*other.y - y*other.x
    );
}

Vec3 CalculateFaceNormal(Vec3 x, Vec3 y, Vec3 z) {
    return (y - x).cross(z - x);
}

/*
 * Vec4
 */

Vec4::Vec4() : x(0), y(0), z(0), w(0) {}

Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}


Vec4 Vec4::operator+(const Vec4& other) const {
    return Vec4(x+other.x, y+other.y, z+other.z, w+other.w);
}

Vec4 Vec4::operator-(const Vec4& other) const {
    return Vec4(x-other.x, y-other.y, z-other.z, w-other.w);
}

Vec4 Vec4::operator*(const float scalar) const {
    return Vec4(x*scalar, y*scalar, z*scalar, w*scalar);
}

void Vec4::operator=(Color c) {
    x = c.r;
    y = c.g;
    z = c.b;
    w = c.a;
}

float Vec4::length() const {
    return sqrt(x*x + y*y + z*z + w*w);
}

float Vec4::dot(const Vec4& other) const {
    return x*other.x + y*other.y + z*other.z + w*other.w;
}

Vec4 Vec4::normalize() const {
    float len = length();
    return Vec4(x / len, y / len, z / len, w / len);
}

/*
 * Color
 */

Color::Color() : r(0.), g(0.), b(0.), a(0.) {}
Color::Color(const float val) : r(val), g(val), b(val), a(val) {}
Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
