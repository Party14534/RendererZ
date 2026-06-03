#include "../Primitives.h"

/*
 * TRIANGLE
 */

Tri::Tri() : Drawable(_defaultVerts) { }

void Tri::init() {
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

    // vertex col
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                            8 * sizeof(float),
                            (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // tex coord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                            8 * sizeof(float),
                            (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    initialized = true;
}

void Tri::draw(Shader& defaultShader, const Mat& viewMat, const Mat& projMat) {
    if (!initialized) init();

    if (shader == nullptr) {
        defaultShader.use();
        defaultShader.setColor(SHADER_COLOR_UNIFORM, color);
        defaultShader.setBool(SHADER_TEX_SET_UNIFORM, texs.size() > 0);
        defaultShader.setMat4(SHADER_MODEL_SET_UNIFORM, getModelMat());
    } else {
        shader->use();
        shader->setMat4(SHADER_MODEL_SET_UNIFORM, getModelMat());
        shader->setMat4(SHADER_VIEW_SET_UNIFORM, viewMat);
        shader->setMat4(SHADER_PROJECTION_SET_UNIFORM, projMat);
    }

    for(int i = 0; i < texs.size(); i++) {
        texs[i]->bind(i); 
    }
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    
    // Unbind textures
    for(int i = 0; i < texs.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        //glBindTexture(GL_TEXTURE_2D, 0);
    }
}

