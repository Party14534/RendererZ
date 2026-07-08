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
#define SHADER_MATERIAL_REFLECTIVITY_UNIFORM "material_z.reflectivity"
#define SHADER_COLOR_UNIFORM "color_z"
#define SHADER_TEX_SET_UNIFORM "usingTex_z"
#define SHADER_SKYBOX_SET_UNIFORM "usingSkyBox_z"
#define SHADER_TEX_UNIFORM "tex"
#define SHADER_SKYBOX_UNIFORM "skyBox_z"
// Cube map lives on a high, dedicated unit so it never collides with the
// sampler2D `tex` (which uses unit 0). Two different sampler types sharing a
// unit is a GL_INVALID_OPERATION (1282) at draw time.
#define SKYBOX_TEXTURE_UNIT 8
#define SHADER_MODEL_SET_UNIFORM "model_z"
#define SHADER_VIEW_SET_UNIFORM "view_z"
#define SHADER_PROJECTION_SET_UNIFORM "projection_z"
#define SHADER_VIEW_POSITION_UNIFORM "view_pos_z"
#define SHADER_DIRECTIONAL_LIGHT "dirLight_z"
#define SHADER_POINT_LIGHT_COUNT "pointLightCnt_z"

struct Material {
    Color color;
    float ambient = 1.f;
    float diffuse = 1.f;
    float specular = 1.f;
    float shininess = 32.f;
    float reflectivity = 0.f;
};

struct DirLightProperties {
    float ambient = 1.f;
    float diffuse = 1.f;
    float specular = 1.f;
};

struct PointLightProperties {
    float ambient = 1.f;
    float diffuse = 1.f;
    float specular = 1.f;
    Vec3 attenuation = Vec3(1.f, .09f, .032f);
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

    const void setDirLight(const Vec3& dir, const Vec3& ambient,
            const Vec3& diffuse, const Vec3& specular) const;
    const void setPointLight(const std::string& name, const Vec3& pos, const Vec3& ambient,
            const Vec3& diffuse, const Vec3& specular,
            const Vec3& attenuation) const;
};

unsigned int loadShader(std::string path, int shaderType);

std::string GetPointLightName(int i);

#endif
