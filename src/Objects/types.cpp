#include "Types.h"

Vec3::Vec3() : x(0.), y(0.), z(0.) {}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

Vec3 Vec3::operator+(const Vec3 v) {
    return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator-(const Vec3 v) {
    return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec4::Vec4() : x(0.), y(0.), z(0.) {}

Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Vec4 Vec4::operator+(const Vec4 v) {
    return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vec4 Vec4::operator-(const Vec4 v) {
    return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

/*
 * Color
 */
Color::Color() : r(0.), g(0.), b(0.), a(0.) {}
Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
