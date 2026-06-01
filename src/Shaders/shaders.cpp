#include "shaders.h"

std::string loadFile(std::string path) {
    std::ifstream file (path, std::ios::binary);
    std::string code;

    code.assign(std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>());

    return code + '\0';
}

unsigned int loadShader(std::string path, int shaderType) {
    const std::string shaderCode = loadFile(path);

    if (shaderCode.length() <= 10) { 
        std::cout << "Failed to load shader at path: " << path << "\n";
        return 0;
    }

    unsigned int shader = glCreateShader(shaderType);
    const char *source = shaderCode.data();

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Check for error
    int success;
    char log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
         glGetShaderInfoLog(shader, 512, NULL, log);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << log << std::endl;
    }

    return shader;
}

Shader::Shader() {}

Shader::Shader(const std::string& vertPath, const std::string& fragPath) {
    vID = loadShader(vertPath, GL_VERTEX_SHADER);
    fID = loadShader(fragPath, GL_FRAGMENT_SHADER);

    // Link shaders
    ID = glCreateProgram();
    glAttachShader(ID, vID);
    glAttachShader(ID, fID);
    glLinkProgram(ID);
}

const void Shader::use() {
    glUseProgram(ID);
}

const void Shader::setBool(const std::string& name, bool val) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)val);
}

const void Shader::setInt(const std::string& name, int val) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
}

const void Shader::setFloat(const std::string& name, float val) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
}

const void Shader::setVec3(const std::string& name, Vec3 val) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), val.x, val.y, val.z);
}

const void Shader::setVec4(const std::string& name, Vec4 val) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), val.x, val.y, val.z, val.w);
}

const void Shader::setColor(const std::string& name, Color val) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), val.r, val.g, val.b, val.a);
}

const void Shader::setMat4(const std::string& name, const Mat& m) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),
            1, GL_TRUE, m.values.data());

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "open gl error " << err << "\n";
    }
}
