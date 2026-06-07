#ifndef VEC3_H
#define VEC3_H

struct Vec2 {
    float x, y;
    
    Vec2();
    Vec2(const float x, const float y);

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(float scalar) const;
};

struct Vec3 {
    float x, y, z;
    
    Vec3();
    Vec3(float x, float y, float z);

    Vec3 operator+(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;
    Vec3 operator*(float scalar) const;

    float length() const;
    float dot(const Vec3& other) const;
    Vec3 normalize() const;
    Vec3 cross(const Vec3& other) const;
};

struct Color;

struct Vec4 {
    float x, y, z, w;

    Vec4();
    Vec4(float x, float y, float z, float w);

    Vec4 operator+(const Vec4& other) const;
    Vec4 operator-(const Vec4& other) const;
    Vec4 operator*(float scalar) const;
    void operator=(Color c);

    float length() const;
    float dot(const Vec4& other) const;
    Vec4 normalize() const;
};

struct Color {
    float r, g, b, a;
    Color();
    Color(const float val);
    Color(float r, float g, float b, float a);
};

#endif
