#include "shaders.h"

std::string loadFile(std::string path) {
    std::ifstream file (path, std::ios::binary);
    std::string code;

    code.assign(std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>());

    return code + '\0';
}

VertexShader VertexShader::fromPath(std::string path) {
    const std::string shaderCode = loadFile(path);

    VertexShader s;

    if (shaderCode.length() <= 10) { 
        std::cout << "Failed to load shader at path: " << path << "\n";
        s.ID = 0;
        return s;
    }

    s.ID = glCreateShader(GL_VERTEX_SHADER);
    const char *source = shaderCode.data();

    glShaderSource(s.ID, 1, &source, NULL);
    glCompileShader(s.ID);

    // Check for error
    int success;
    char log[512];
    glGetShaderiv(s.ID, GL_COMPILE_STATUS, &success);
    if (!success) {
         glGetShaderInfoLog(s.ID, 512, NULL, log);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << log << std::endl;
    }

    return s;
}

VertexShader VertexShader::fromString(std::string data) {
    VertexShader s;
    s.ID = 0;

    if (data.length() <= 10) { 
        std::cout << "Failed to load shader from string\n";
        return s;
    }

    s.ID = glCreateShader(GL_VERTEX_SHADER);
    const char *source = data.data();

    glShaderSource(s.ID, 1, &source, NULL);
    glCompileShader(s.ID);

    // Check for error
    int success;
    char log[512];
    glGetShaderiv(s.ID, GL_COMPILE_STATUS, &success);
    if (!success) {
         glGetShaderInfoLog(s.ID, 512, NULL, log);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << log << std::endl;
    }

    return s;
}

FragmentShader FragmentShader::fromPath(std::string path) {
    const std::string shaderCode = loadFile(path);

    FragmentShader s;

    if (shaderCode.length() <= 10) { 
        std::cout << "Failed to load shader at path: " << path << "\n";
        s.ID = 0;
        return s;
    }

    s.ID = glCreateShader(GL_FRAGMENT_SHADER);
    const char *source = shaderCode.data();

    glShaderSource(s.ID, 1, &source, NULL);
    glCompileShader(s.ID);

    // Check for error
    int success;
    char log[512];
    glGetShaderiv(s.ID, GL_COMPILE_STATUS, &success);
    if (!success) {
         glGetShaderInfoLog(s.ID, 512, NULL, log);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << log << std::endl;
    }

    return s;
}

FragmentShader FragmentShader::fromString(std::string data) {
    FragmentShader s;
    s.ID = 0;

    if (data.length() <= 10) { 
        std::cout << "Failed to load shader from string\n";
        return s;
    }

    s.ID = glCreateShader(GL_FRAGMENT_SHADER);
    const char *source = data.data();

    glShaderSource(s.ID, 1, &source, NULL);
    glCompileShader(s.ID);

    // Check for error
    int success;
    char log[512];
    glGetShaderiv(s.ID, GL_COMPILE_STATUS, &success);
    if (!success) {
         glGetShaderInfoLog(s.ID, 512, NULL, log);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << log << std::endl;
    }

    return s;
}

static void checkLinkErrors(u32 id) {
    int success;
    char log[512];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, log);
        std::cout << "ERROR::SHADER::LINK_FAILED\n" << log << std::endl;
    }
}

ShaderProgram::ShaderProgram() {}

ShaderProgram::ShaderProgram(const std::string& vertPath, const std::string& fragPath) {
    vert = VertexShader::fromPath(vertPath);
    frag = FragmentShader::fromPath(fragPath);

    // Link shaders
    ID = glCreateProgram();
    glAttachShader(ID, vert.ID);
    glAttachShader(ID, frag.ID);
    glLinkProgram(ID);
    checkLinkErrors(ID);
}

ShaderProgram ShaderProgram::fromStrings(const std::string &vert, const std::string &frag) {
    ShaderProgram s;

    s.vert = VertexShader::fromString(vert);
    s.frag = FragmentShader::fromString(frag);

    // Link shaders
    s.ID = glCreateProgram();
    glAttachShader(s.ID, s.vert.ID);
    glAttachShader(s.ID, s.frag.ID);
    glLinkProgram(s.ID);
    checkLinkErrors(s.ID);

    return s;
}

const void ShaderProgram::use() {
    glUseProgram(ID);
}

const void ShaderProgram::setBool(const std::string& name, bool val) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)val);
}

const void ShaderProgram::setInt(const std::string& name, int val) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
}

const void ShaderProgram::setFloat(const std::string& name, float val) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
}

const void ShaderProgram::setVec2(const std::string& name, Vec2 val) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), val.x, val.y);
}

const void ShaderProgram::setVec3(const std::string& name, Vec3 val) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), val.x, val.y, val.z);
}

const void ShaderProgram::setVec4(const std::string& name, Vec4 val) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), val.x, val.y, val.z, val.w);
}

const void ShaderProgram::setColor(const std::string& name, Color val) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), val.r, val.g, val.b, val.a);
}

const void ShaderProgram::setMat4(const std::string& name, const Mat& m) const {
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

const void ShaderProgram::setMaterial(const Material& m) const {
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

const void ShaderProgram::setDirLight(const Vec3& dir, const Vec3& ambient,
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

const void ShaderProgram::bindUniformBlock(const std::string& name, u32 bindingPoint) const {
    u32 blockIndex = glGetUniformBlockIndex(ID, name.c_str());
    glUniformBlockBinding(ID, blockIndex, bindingPoint);
}
