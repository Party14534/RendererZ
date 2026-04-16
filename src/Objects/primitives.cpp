#include "Primitives.h"

Tri::Tri(std::vector<Vec3> _vertices) : Drawable(_vertices) { }

void Tri::init() {
    // Generate VAO and VBO
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    // Bind and set up buffers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vec3),
            vertices.data(),
            GL_STATIC_DRAW);

    // Set data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    ColorLoc = glGetUniformLocation(baseShaderProgram,
                                            SHADER_COLOR_UNIFORM);

    initialized = true;
}

void Tri::draw() {
    if (!initialized) init();

    glUseProgram(baseShaderProgram);
    glUniform4f(ColorLoc, color.r, color.g, color.b, color.a);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

Rect::Rect(std::vector<Vec3> _vertices, std::vector<u32> _indices)
    : Drawable(_vertices, _indices) { }

void Rect::init() {
    // Generate VAO and VBO and EBO
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    // Bind and set up buffers
    glBindVertexArray(VAO); // MUST BIND VAO FIRST

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vec3),
            vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32),
            indices.data(), GL_STATIC_DRAW);

    // Set data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                            sizeof(Vec3), (void*)0);
    glEnableVertexAttribArray(0);

    ColorLoc = glGetUniformLocation(baseShaderProgram,
                                            SHADER_COLOR_UNIFORM);

    initialized = true;
}

void Rect::draw() {
    if (!initialized) init();

    glUseProgram(baseShaderProgram);
    glUniform4f(ColorLoc, color.r, color.g, color.b, color.a);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
