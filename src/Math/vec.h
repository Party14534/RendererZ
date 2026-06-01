#ifndef VEC3_H
#define VEC3_H

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

struct Vec4 {
    float x, y, z, w;

    Vec4();
    Vec4(float x, float y, float z, float w);

    Vec4 operator+(const Vec4& other) const;
    Vec4 operator-(const Vec4& other) const;
    Vec4 operator*(float scalar) const;

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
