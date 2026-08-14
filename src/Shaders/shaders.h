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
#define SHADER_TEX_UNIFORM "tex"
#define SHADER_NORMAL_MAP_SET_UNIFORM "usingNormalMap_z"
#define SHADER_NORMAL_MAP_UNIFORM "normalMap"
#define SHADER_SKYBOX_SET_UNIFORM "usingSkyBox_z"
#define SHADER_SKYBOX_UNIFORM "skyBox_z"
#define SKYBOX_TEXTURE_UNIT 8
#define SHADER_MODEL_SET_UNIFORM "model_z"
#define SHADER_RESOLUTION_UNIFORM "resolution_z"
#define SHADER_VIEW_SET_UNIFORM "view_z"
#define SHADER_PROJECTION_SET_UNIFORM "projection_z"
#define SHADER_VIEW_POSITION_UNIFORM "view_pos_z"
#define SHADER_DIRECTIONAL_LIGHT "dirLight_z"
#define SHADER_POINT_LIGHT_COUNT "pointLightCnt_z"
#define SHADER_POINT_SIZE_UNIFORM "pointSize_z"
#define SHADER_UV_SCALE_UNIFORM "uvScale_z"
#define SHADER_SPECULAR_UNIFORM "specular_z"
#define SHADER_GPOSITION_UNIFORM "gPosition"
#define SHADER_GNORMAL_UNIFORM "gNormal"
#define SHADER_GALBEDO_SPEC_UNIFORM "gAlbedoSpec"
#define SHADER_GSAO_UNIFORM "gSAO"
#define SHADER_GSAO_BLUR_UNIFORM "gSAOBlur"
#define SHADER_POINT_LIGHT_BLOCK "PointLightBlock"
#define POINT_LIGHT_UBO_BINDING 0
#define MAX_POINT_LIGHTS 800 // keep in sync with lightPassFrag.frag; 65536-byte UBO limit / 80 bytes per light = 819 max

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

class Shader {
    public:
        u32 ID;
};

class VertexShader : public Shader {
    public:
        static VertexShader fromPath(std::string path);
        static VertexShader fromString(std::string data);
};

class FragmentShader : public Shader {
    public:
        static FragmentShader fromPath(std::string path);
        static FragmentShader fromString(std::string data);
};

struct ShaderProgram {
    // Shader program id
    u32 ID;
    VertexShader vert;
    FragmentShader frag;

    ShaderProgram();
    ShaderProgram(VertexShader vert, FragmentShader frag);
    ShaderProgram(const std::string& vertPath, const std::string& fragPath);

    // Use the shader
    const void use();

    // Set uniforms
    const void setBool(const std::string& name, bool val) const;
    const void setInt(const std::string& name, int val) const;
    const void setFloat(const std::string& name, float val) const;

    const void setVec2(const std::string& name, Vec2 val) const;
    const void setVec3(const std::string& name, Vec3 val) const;
    const void setVec4(const std::string& name, Vec4 val) const;
    const void setColor(const std::string& name, Color val) const;

    const void setMat4(const std::string& name, const Mat& m) const;

    const void setMaterial(const Material& m) const;

    const void setDirLight(const Vec3& dir, const Vec3& ambient,
            const Vec3& diffuse, const Vec3& specular) const;

    const void bindUniformBlock(const std::string& name, u32 bindingPoint) const;

    static ShaderProgram fromStrings(const std::string& vert, const std::string& frag);
};

#endif
