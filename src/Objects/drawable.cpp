#include "Drawable.h"

/*
 * Complex Drawable
 */

ComplexDrawable::ComplexDrawable(std::vector<Drawable>& targets) : targets(targets) { }

ComplexDrawable::~ComplexDrawable() { }

void ComplexDrawable::draw(std::shared_ptr<Shader> defaultShader) {
    for(auto& target : targets) {
        target.draw(defaultShader);
    }
}

/*
 * Drawable
 */

Drawable::Drawable(std::shared_ptr<Mesh> m) : 
    mesh(m) { }

void Drawable::setColor(Color c) { material.color = c; }
Color Drawable::getColor() const { return material.color; }

void Drawable::setPos(Vec3 v) { transform.pos = v; }
Vec3 Drawable::getPos() const { return transform.pos; }

void Drawable::setScale(Vec3 s) { transform.scale = s; }
Vec3 Drawable::getScale() const { return transform.scale; }

void Drawable::setMaterial(Material m) { material = m; }
Material Drawable::getMaterial() const { return material; }

void Drawable::rotateX(float angle) { 
    transform.rotation.x = angle;
}

void Drawable::rotateY(float angle) { 
    transform.rotation.y = angle;
}

void Drawable::rotateZ(float angle) { 
    transform.rotation.z = angle;
}

void Drawable::setDiffuseTexture(std::shared_ptr<Texture> _tex) {
    diffuseTexture = _tex;
}

void Drawable::setNormalTexture(std::shared_ptr<Texture> _tex) {
    normalTexture = _tex;
}

void Drawable::setShader(std::shared_ptr<Shader> _shader) {
    shader = _shader;
}

void Drawable::setUVScale(Vec2 scale) { uvScale = scale; }
Vec2 Drawable::getUVScale() const { return uvScale; }

void Drawable::draw(std::shared_ptr<Shader> defaultShader) {
    if (shader == nullptr) {
        defaultShader->use();
        defaultShader->setMat4(SHADER_MODEL_SET_UNIFORM, transform.GetModelMat());
        defaultShader->setMaterial(material);
        defaultShader->setBool(SHADER_TEX_SET_UNIFORM, diffuseTexture != nullptr);
        defaultShader->setBool(SHADER_NORMAL_MAP_SET_UNIFORM, normalTexture != nullptr);
        defaultShader->setVec2(SHADER_UV_SCALE_UNIFORM, uvScale);
    } else { shader->use(); }

    if (diffuseTexture != nullptr) { diffuseTexture->bind(0); }
    if (normalTexture != nullptr) { normalTexture->bind(1); }
    mesh->draw();
}

Mat Transform::GetModelMat() {
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
