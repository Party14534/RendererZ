#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <fstream>
#include <iterator>
#include <iostream>
#include <unordered_map>

#include "../Math/math.h"
#include "../System/Buffer/Buffer.h"
#include "Material.h"
#include "ShaderUniformsNames.h"
                        
inline std::unordered_map<std::string, u32> uniformIDs;

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

    const void setMat4(const std::string& name, const Mat4& m) const;
    const void setMat4(const std::string& name, const Mat4D& m) const;

    //const void setBlinnPhongMaterial(const BlinnPhongMaterial& m) const;
    const void setPBRMaterial(const PBRMaterial& m) const;

    const void setDirLight(const Vec3& dir, const Vec3& ambient,
            const Vec3& diffuse, const Vec3& specular) const;

    const void bindUniformBlock(const std::string& name, UniformBuffer& ubo) const;

    static ShaderProgram fromStrings(const std::string& vert, const std::string& frag);

    const friend u32 getUniformLocation(const u32 ID, const std::string& name);
};

#endif
