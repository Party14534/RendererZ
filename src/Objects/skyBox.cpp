#include "SkyBox.h"
#include "Objects/Texture.h"
#include "Shaders/shaders.h"

SkyBox::SkyBox(std::vector<std::string> textures) :
    Drawable(_defaultVerts),
    map(textures) { }

void SkyBox::init() {
    // Generate VAO and VBO
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    // Bind and set up buffers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexAttribute),
            vertices.data(), GL_STATIC_DRAW);

    // Set attribs
    // vertex pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                            8 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);

    // vertex normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                            8 * sizeof(float),
                            (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // uv
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                            8 * sizeof(float),
                            (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    initialized = true;
}

void SkyBox::draw(Shader* shader, Shader& defaultShader,
        const Mat& viewMat,
        const Mat& projMat, const Vec3& viewPos,
        const DirLight& dLight,
        const std::vector<PointLight>& pLights,
        const CubeMap* skyBox) {
    if (!initialized) init();

    // Use shader
    defaultShader.use();
    defaultShader.setMat4(SHADER_VIEW_SET_UNIFORM, viewMat);
    defaultShader.setMat4(SHADER_PROJECTION_SET_UNIFORM, projMat);

    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);

    // Bind cube map
    map.bind();

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, _defaultVerts.size());
    glBindVertexArray(0);

    // Unbind texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    // Restore default state for the next frame's object rendering
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
}
