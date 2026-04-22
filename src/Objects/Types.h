#ifndef TYPES_H
#define TYPES_H

#include <vector>

typedef unsigned int u32;

struct Vec2 {
    float x, y;

    Vec2();
    Vec2(float x, float y);
    Vec2 operator +(const Vec2 v);
    Vec2 operator -(const Vec2 v);
};

struct Vec3 {
    float x, y, z;

    Vec3();
    Vec3(float x, float y, float z);
    Vec3 operator +(const Vec3 v);
    Vec3 operator -(const Vec3 v);
};

struct Vec4 {
    float x, y, z, w;

    Vec4();
    Vec4(float x, float y, float z, float w);
    Vec4 operator +(const Vec4 v);
    Vec4 operator -(const Vec4 v);
};

struct Color {
    float r, g, b, a;
    Color();
    Color(float r, float g, float b, float a);
};

#endif
