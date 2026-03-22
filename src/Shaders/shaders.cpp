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
