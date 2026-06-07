#include "LightSource.h"

/*
 * Light Source
 */

LightSource::LightSource() : Drawable() { 
    isLightSource = true; 
    color = Color(1.);
}
LightSource::LightSource(std::vector<VertexAttribute> verts,
        std::vector<u32> indices) : Drawable(verts, indices) {
    isLightSource = true; 
    color = Color(1.);
}

void LightSource::init() {
    // Don't create stuff on the GPU if theres no model to load
    if (vertices.empty()) { return; }

    // Generate VAO and VBO and EBO
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    // Bind and set up buffers
    glBindVertexArray(VAO); // MUST BIND VAO FIRST

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexAttribute),
            vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32),
            indices.data(), GL_STATIC_DRAW);

    // Set attribs
    // vertex pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                            8 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    initialized = true;
}

void LightSource::draw(Shader& defaultShader, const Mat& viewMat, const Mat& projMat, const Vec3& viewPos) {
    if (vertices.empty()) { return; }
    if (!initialized) init();

    if (shader == nullptr) {
        defaultShader.use();
        defaultShader.setColor(SHADER_COLOR_UNIFORM, color);
        defaultShader.setMat4(SHADER_MODEL_SET_UNIFORM, getModelMat());
        defaultShader.setMat4(SHADER_VIEW_SET_UNIFORM, viewMat);
        defaultShader.setMat4(SHADER_PROJECTION_SET_UNIFORM, projMat);
    } else {
        shader->use();
        Mat m = getModelMat();
        shader->setMat4(SHADER_MODEL_SET_UNIFORM, getModelMat());
        shader->setMat4(SHADER_VIEW_SET_UNIFORM, viewMat);
        shader->setMat4(SHADER_PROJECTION_SET_UNIFORM, projMat);
    }

    /* TODO: support textures
    for(int i = 0; i < texs.size(); i++) {
        texs[i]->bind(i); 
    }*/
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    /* Unbind textures
    for(int i = 0; i < texs.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }*/
}
