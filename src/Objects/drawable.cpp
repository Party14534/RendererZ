#include "Drawable.h"

/*
 * Complex Drawable
 */

ComplexDrawable::ComplexDrawable(std::vector<Drawable>& targets) : targets(targets) { }

ComplexDrawable::~ComplexDrawable() { }

void ComplexDrawable::draw(std::shared_ptr<ShaderProgram> defaultShader, const Mat4D& vp) {
    for(auto& target : targets) {
        target.draw(defaultShader, vp);
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

/*void Drawable::setShader(std::shared_ptr<Shader> _shader) {
    shader = _shader;
}*/

void Drawable::setUVScale(Vec2 scale) { uvScale = scale; }
Vec2 Drawable::getUVScale() const { return uvScale; }

void Drawable::draw(std::shared_ptr<ShaderProgram> shader, const Mat4D& vp) {
    Mat4D mvp = vp * transform.GetModelMatD();
    shader->setMat4(SHADER_MODEL_SET_UNIFORM, transform.GetModelMat());
    shader->setMat4(SHADER_MVP_SET_UNIFORM, mvp);
    shader->setVec3(SHADER_COLOR_UNIFORM, material.color.toRGB());
    shader->setFloat(SHADER_SPECULAR_UNIFORM, material.specular);
    shader->setBool(SHADER_TEX_SET_UNIFORM, diffuseTexture != nullptr);
    shader->setBool(SHADER_NORMAL_MAP_SET_UNIFORM, normalTexture != nullptr);
    shader->setVec2(SHADER_UV_SCALE_UNIFORM, uvScale);

    if (diffuseTexture != nullptr) { diffuseTexture->setActive(0); }
    if (normalTexture != nullptr) { normalTexture->setActive(1); }
    mesh->draw();
}

void Drawable::drawLightPass(std::shared_ptr<ShaderProgram> shader) {
    shader->setMat4(SHADER_MODEL_SET_UNIFORM, transform.GetModelMat());

    mesh->draw();
}

Mat4 Transform::GetModelMat() {
    // Rotations
    float cx = cos(rotation.x);
    float sx = sin(rotation.x);
    float cy = cos(rotation.y);
    float sy = sin(rotation.y);
    float cz = cos(rotation.z);
    float sz = sin(rotation.z);

    // Combined rotation matrix (Z * Y * X)
    float r00 = cy * cz,                r01 = -cy * sz,               r02 = sy;
    float r10 = cx * sz + cz * sx * sy, r11 = cx * cz - sx * sy * sz, r12 = -cy * sx;
    float r20 = sx * sz - cx * cz * sy, r21 = cz * sx + cx * sy * sz, r22 = cx * cy;

    // Combine scale and rotation: scale is diagonal, so rotMat * scaleMat is
    // just each rotation column scaled by the matching scale component.
    return Mat4({
        r00 * scale.x, r01 * scale.y, r02 * scale.z, pos.x,
        r10 * scale.x, r11 * scale.y, r12 * scale.z, pos.y,
        r20 * scale.x, r21 * scale.y, r22 * scale.z, pos.z,
        0.f,           0.f,           0.f,           1.f
    });
}

Mat4D Transform::GetModelMatD() {
    // Rotations
    float cx = cos(rotation.x);
    float sx = sin(rotation.x);
    float cy = cos(rotation.y);
    float sy = sin(rotation.y);
    float cz = cos(rotation.z);
    float sz = sin(rotation.z);

    // Combined rotation matrix (Z * Y * X)
    float r00 = cy * cz,                r01 = -cy * sz,               r02 = sy;
    float r10 = cx * sz + cz * sx * sy, r11 = cx * cz - sx * sy * sz, r12 = -cy * sx;
    float r20 = sx * sz - cx * cz * sy, r21 = cz * sx + cx * sy * sz, r22 = cx * cy;

    // Combine scale and rotation: scale is diagonal, so rotMat * scaleMat is
    // just each rotation column scaled by the matching scale component.
    return Mat4D({
        r00 * scale.x, r01 * scale.y, r02 * scale.z, pos.x,
        r10 * scale.x, r11 * scale.y, r12 * scale.z, pos.y,
        r20 * scale.x, r21 * scale.y, r22 * scale.z, pos.z,
        0.,            0.,            0.,            1.
    });
}
