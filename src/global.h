#ifndef GLOBAL_H
#define GLOBAL_H

#include "../include/glad.h"

#include <GLFW/glfw3.h>
#include "Shaders/shaders.h"
#include <iostream>

#define SHADER_COLOR_UNIFORM "color_z"

inline GLFWwindow* win;

void initializeGL();

#endif
