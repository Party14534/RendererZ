#include "Drawable.h"
#include "Shaders/shaders.h"
#include "global.h"

Drawable::Drawable() : modelMat(Mat(4, 4)) { }

Drawable::Drawable(std::vector<VertexAttribute> _vertices) :
    vertices(_vertices), modelMat(Mat(4, 4)) { }

Drawable::Drawable(std::vector<VertexAttribute> _vertices, std::vector<u32> _indices) :
    vertices(_vertices), indices(_indices), modelMat(Mat(4, 4)) { }

Drawable::~Drawable() { }

void Drawable::setColor(Color c) { material.color = c; }
Color Drawable::getColor() const { return material.color; }

void Drawable::setPos(Vec3 v) { pos = v; }
Vec3 Drawable::getPos() const { return pos; }

void Drawable::setScale(Vec3 s) { scale = s; }
Vec3 Drawable::getScale() const { return scale; }

void Drawable::setMaterial(Material m) { material = m; }
Material Drawable::getMaterial() const { return material; }

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

void Drawable::setDefaultUniforms(Shader& shader, const Mat& viewMat, const Mat& projMat, const Vec3& viewPos) {
    Vec3 lc = Vec3(lightCol.r, lightCol.g, lightCol.b);
    shader.setLight(lightPos, lc * lightProp.ambient,
                    lc * lightProp.diffuse, lc * lightProp.specular);
    shader.setBool(SHADER_TEX_SET_UNIFORM, texs.size() > 0);
    shader.setMat4(SHADER_MODEL_SET_UNIFORM, getModelMat());
    shader.setMat4(SHADER_VIEW_SET_UNIFORM, viewMat);
    shader.setMat4(SHADER_PROJECTION_SET_UNIFORM, projMat);
    shader.setVec3(SHADER_VIEW_POSITION_UNIFORM, viewPos);
    shader.setMaterial(material);
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
    : x(0), y(0), z(0), xn(0), yn(0), zn(0), u(0), v(0) {}

VertexAttribute::VertexAttribute(
        float x, float y, float z, 
        float xn,float yn,float zn,
        float u, float v)
    : x(x), y(y), z(z), xn(xn), yn(yn), zn(zn), u(u), v(v) {}

