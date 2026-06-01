#ifndef SHADERS_H
#define SHADERS_H

#include "../../include/glad.h"
#include <string>
#include <fstream>
#include <iterator>
#include <iostream>
#include <GLFW/glfw3.h>

#include "../Math/math.h"
#include "../Objects/Texture.h"

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
};

unsigned int loadShader(std::string path, int shaderType);

#endif
