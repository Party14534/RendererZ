#ifndef SHADERS_H
#define SHADERS_H

#include "../../include/glad.h"
#include <string>
#include <fstream>
#include <iterator>
#include <iostream>
#include <GLFW/glfw3.h>

#include "../Math/math.h"

#define SHADER_MATERIAL_AMBIENT_UNIFORM "material_z.ambient"
#define SHADER_MATERIAL_DIFFUSE_UNIFORM "material_z.diffuse"
#define SHADER_MATERIAL_SPECULAR_UNIFORM "material_z.specular"
#define SHADER_MATERIAL_SHININESS_UNIFORM "material_z.shininess"
#define SHADER_LIGHT_POSITION_UNIFORM "light_z.pos"
#define SHADER_LIGHT_AMBIENT_UNIFORM "light_z.ambient"
#define SHADER_LIGHT_DIFFUSE_UNIFORM "light_z.diffuse"
#define SHADER_LIGHT_SPECULAR_UNIFORM "light_z.specular"
#define SHADER_COLOR_UNIFORM "color_z"
#define SHADER_TEX_SET_UNIFORM "usingTex_z"
#define SHADER_MODEL_SET_UNIFORM "model_z"
#define SHADER_VIEW_SET_UNIFORM "view_z"
#define SHADER_PROJECTION_SET_UNIFORM "projection_z"
#define SHADER_VIEW_POSITION_UNIFORM "view_pos_z"

struct Material {
    Color color;
    float ambient = 1.f;
    float diffuse = 1.f;
    float specular = 1.f;
    float shininess = 32.f;
};

struct LightProperties {
    float ambient = 1.f;
    float diffuse = 1.f;
    float specular = 1.f;
};

struct Shader {
    // Shader program id
    u32 ID, vID, fID;

    Shader();
    Shader(const std::string& vertPath, const std::string& fragPath);

    // Use the shader
    const void use();

    // Set uniforms
    const void setBool(const std::string& name, bool val) const;
    const void setInt(const std::string& name, int val) const;
    const void setFloat(const std::string& name, float val) const;

    const void setVec3(const std::string& name, Vec3 val) const;
    const void setVec4(const std::string& name, Vec4 val) const;
    const void setColor(const std::string& name, Color val) const;

    const void setMat4(const std::string& name, const Mat& m) const;

    const void setMaterial(const Material& m) const;
    const void setLight(const Vec3& pos, const Vec3& ambient,
            const Vec3& diffuse, const Vec3& specular) const;
};

unsigned int loadShader(std::string path, int shaderType);

#endif
