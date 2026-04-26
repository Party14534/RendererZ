#ifndef TYPES_H
#define TYPES_H

#include <ostream>
#include <vector>
#include <array>
#include <cmath>
#include <cstring>

typedef unsigned int u32;

struct Vec2 {
    float x, y;

    Vec2();
    Vec2(float x, float y);
    Vec2 operator +(const Vec2 v) const;
    Vec2 operator -(const Vec2 v) const;
};

struct Vec3 {
    float x, y, z;

    Vec3();
    Vec3(float x, float y, float z);
    Vec3 operator +(const Vec3 v) const;
    Vec3 operator -(const Vec3 v) const;
    
    float length() const;
    Vec3 normalize() const;
    float dot(Vec3 vec) const;
    Vec3 cross(Vec3 vec) const;
};

struct Vec4 {
    float x, y, z, w;

    Vec4();
    Vec4(float x, float y, float z, float w);
    Vec4 operator +(const Vec4 v) const;
    Vec4 operator -(const Vec4 v) const;
};

struct Mat4 {
    std::array<float, 16> data;

    Mat4();
    Mat4(const std::array<float, 16> _data);

    float& operator [](u32 i);
    Mat4 operator +(const float m) const;
    Mat4 operator -(const float m) const;
    Mat4 operator *(const float m) const;
    Mat4 operator /(const float m) const;
    Mat4 operator +(const Mat4 m) const;
    Mat4 operator -(const Mat4 m) const;
    Mat4 operator *(const Mat4 m) const;

    friend std::ostream& operator <<(std::ostream& os, const Mat4& m);

    float get(u32 i, u32 j) const;
};

struct Color {
    float r, g, b, a;
    Color();
    Color(float r, float g, float b, float a);
};

#endif
