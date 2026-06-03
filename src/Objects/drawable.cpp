#include "Drawable.h"

Drawable::Drawable() : modelMat(Mat(4, 4)) { }

Drawable::Drawable(std::vector<VertexAttribute> _vertices) :
    vertices(_vertices), modelMat(Mat(4, 4)) { }

Drawable::Drawable(std::vector<VertexAttribute> _vertices, std::vector<u32> _indices) :
    vertices(_vertices), indices(_indices), modelMat(Mat(4, 4)) { }

Drawable::~Drawable() { }

void Drawable::setColor(Color c) { color = c; }
Color Drawable::getColor() { return color; }

void Drawable::setPos(Vec3 v) { pos = v; }
Vec3 Drawable::getPos() { return pos; }

void Drawable::setScale(Vec3 s) { scale = s; }
Vec3 Drawable::getScale() { return scale; }

void Drawable::rotateX(float angle) { 
    rotation.x = angle;
}

void Drawable::rotateY(float angle) { 
    rotation.y = angle;
}

void Drawable::rotateZ(float angle) { 
    rotation.z = angle;
}

void Drawable::setTexture(Texture& _tex) {
    if (texs.size() == 0) {
        texs.push_back(&_tex);
    } else {
        texs[0] = &_tex;
    }
}

void Drawable::addTexture(Texture& _tex) {
    texs.push_back(&_tex);
}

void Drawable::setShader(Shader& _shader) {
    shader = &_shader;
}

Mat Drawable::getModelMat() {
    // Build scale matrix
    Mat scaleMat = Mat::getIdentity(4);
    scaleMat.set(0, 0, scale.x);
    scaleMat.set(1, 1, scale.y);
    scaleMat.set(2, 2, scale.z);
    
    // Build rotation matrix
    Mat rotMat = Mat::getIdentity(4);
    
    // Rotations
    float cx = cos(rotation.x);
    float sx = sin(rotation.x);
    float cy = cos(rotation.y);
    float sy = sin(rotation.y);
    float cz = cos(rotation.z);
    float sz = sin(rotation.z);
    
    // Combined rotation matrix (Z * Y * X)
    rotMat.set(0, 0, cy * cz);
    rotMat.set(0, 1, -cy * sz);
    rotMat.set(0, 2, sy);
    
    rotMat.set(1, 0, cx * sz + cz * sx * sy);
    rotMat.set(1, 1, cx * cz - sx * sy * sz);
    rotMat.set(1, 2, -cy * sx);
    
    rotMat.set(2, 0, sx * sz - cx * cz * sy);
    rotMat.set(2, 1, cz * sx + cx * sy * sz);
    rotMat.set(2, 2, cx * cy);
    
    // Combine scale and rotation
    Mat m = rotMat * scaleMat;
    
    // Apply translation
    m.set(0, 3, pos.x);
    m.set(1, 3, pos.y);
    m.set(2, 3, pos.z);
    
    return m;
}

/*
 * Helper types
 */

VertexAttribute::VertexAttribute() 
    : x(0), y(0), z(0), r(0), g(0), b(0), u(0), v(0) {}

VertexAttribute::VertexAttribute(
        float x, float y, float z, 
        float r, float g, float b,
        float u, float v)
    : x(x), y(y), z(z), r(r), g(g), b(b), u(u), v(v) {}

