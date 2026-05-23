#ifndef GLOBAL_H
#define GLOBAL_H

#include "../include/glad.h"

#include <GLFW/glfw3.h>
#include <iostream>

#define SHADER_COLOR_UNIFORM "color_z"
#define SHADER_TEX_SET_UNIFORM "usingTex_z"
#define SHADER_TRANSFORM_SET_UNIFORM "transform_z"

inline GLFWwindow* win;

void initializeGL();

#endif
