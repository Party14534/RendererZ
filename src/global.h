#ifndef GLOBAL_H
#define GLOBAL_H

#include "../include/glad.h"

#include <GLFW/glfw3.h>
#include <iostream>

#define SHADER_COLOR_UNIFORM "color_z"
#define SHADER_TEX_SET_UNIFORM "usingTex_z"
#define SHADER_MODEL_SET_UNIFORM "model_z"
#define SHADER_VIEW_SET_UNIFORM "view_z"
#define SHADER_PROJECTION_SET_UNIFORM "projection_z"
#define PI 3.141592653589

inline GLFWwindow* win;

void initializeGL();

#endif
