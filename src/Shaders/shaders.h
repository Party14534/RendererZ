#ifndef SHADERS_H
#define SHADERS_H

#include "../../include/glad.h"
#include <string>
#include <fstream>
#include <iterator>
#include <iostream>
#include <GLFW/glfw3.h>

#include "../Objects/Types.h"

struct Shader {
    // Shader program id
    u32 ID, vID, fID;

    Shader();
    Shader(const std::string& vertPath, const std::string& fragPath);

    // Use the shader
    void use();

    // Set uniforms
    void setBool(const std::string& name, bool val) const;
    void setInt(const std::string& name, int val) const;
    void setFloat(const std::string& name, float val) const;

    void setVec3(const std::string& name, Vec3 val) const;
    void setVec4(const std::string& name, Vec4 val) const;
    void setColor(const std::string& name, Color val) const;
};

unsigned int loadShader(std::string path, int shaderType);

#endif
