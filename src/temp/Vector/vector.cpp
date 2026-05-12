#include "../Types.h"

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

