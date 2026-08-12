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

unsigned int loadShaderFromString(std::string shaderCode, int shaderType) {
    if (shaderCode.length() <= 10) { 
        std::cout << "Failed to load shader from string\n";
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

Shader Shader::fromStrings(const std::string &vert, const std::string &frag) {
    Shader s;

    s.vID = loadShaderFromString(vert, GL_VERTEX_SHADER);
    s.fID = loadShaderFromString(frag, GL_FRAGMENT_SHADER);

    // Link shaders
    s.ID = glCreateProgram();
    glAttachShader(s.ID, s.vID);
    glAttachShader(s.ID, s.fID);
    glLinkProgram(s.ID);

    return s;
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

const void Shader::setVec2(const std::string& name, Vec2 val) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), val.x, val.y);
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
    // TODO: Return number error and don't just exit
    if (m.cols != m.rows || m.cols != 4) {
        std::cerr << "Matrix is not 4x4\n";
        exit(1);
    }

    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),
            1, GL_TRUE, m.values.data());

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "open gl error " << err << "\n";
    }
}

const void Shader::setMaterial(const Material& m) const {
    Vec3 c = m.color.toRGB();
    Vec3 ambient = c * m.ambient;
    Vec3 diffuse = c * m.diffuse;
    Vec3 specular = c * m.specular;

    glUniform3f(glGetUniformLocation(ID, SHADER_MATERIAL_AMBIENT_UNIFORM),
            ambient.x, ambient.y, ambient.z);
    glUniform3f(glGetUniformLocation(ID, SHADER_MATERIAL_DIFFUSE_UNIFORM),
            diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(glGetUniformLocation(ID, SHADER_MATERIAL_SPECULAR_UNIFORM),
            specular.x, specular.y, specular.z);
    glUniform1f(glGetUniformLocation(ID, SHADER_MATERIAL_SHININESS_UNIFORM),
            m.shininess);
    glUniform1f(glGetUniformLocation(ID, SHADER_MATERIAL_REFLECTIVITY_UNIFORM),
            m.reflectivity);
}

const void Shader::setDirLight(const Vec3& dir, const Vec3& ambient,
        const Vec3& diffuse, const Vec3& specular) const {
    std::string name = SHADER_DIRECTIONAL_LIGHT;
    glUniform3f(glGetUniformLocation(ID, (name + ".direction").c_str()),
            dir.x, dir.y, dir.z);
    glUniform3f(glGetUniformLocation(ID, (name + ".ambient").c_str()),
            ambient.x, ambient.y, ambient.z);
    glUniform3f(glGetUniformLocation(ID, (name + ".diffuse").c_str()),
            diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(glGetUniformLocation(ID, (name + ".specular").c_str()),
            specular.x, specular.y, specular.z);
}

const void Shader::setPointLight(const std::string& name, const Vec3& pos, const Vec3& ambient, const Vec3& diffuse,
            const Vec3& specular, const Vec3& attenuation) const {
    glUniform3f(glGetUniformLocation(ID, (name + ".pos").c_str()),
            pos.x, pos.y, pos.z);
    glUniform3f(glGetUniformLocation(ID, (name + ".ambient").c_str()),
            ambient.x, ambient.y, ambient.z);
    glUniform3f(glGetUniformLocation(ID, (name + ".diffuse").c_str()),
            diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(glGetUniformLocation(ID, (name + ".specular").c_str()),
            specular.x, specular.y, specular.z);
    glUniform3f(glGetUniformLocation(ID,(name + ".attenuation").c_str()),
            attenuation.x, attenuation.y, attenuation.z);
}

std::string GetPointLightName(int i) {
    return "pointLights_z[" + std::to_string(i) + "]";
}
