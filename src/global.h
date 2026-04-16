#ifndef GLOBAL_H
#define GLOBAL_H

#include "../include/glad.h"

#include <GLFW/glfw3.h>
#include <iostream>

#define SHADER_COLOR_UNIFORM "color_z"

inline GLFWwindow* win;
inline unsigned int baseShaderProgram;

void initializeGL();

#endif
