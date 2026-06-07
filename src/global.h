#ifndef GLOBAL_H
#define GLOBAL_H

#include "../include/glad.h"

#include <GLFW/glfw3.h>
#include "Math/vec.h"
#include <cstdint>
#include <iostream>

#define SHADER_COLOR_UNIFORM "color_z"
#define SHADER_LIGHT_COLOR_UNIFORM "light_color_z"
#define SHADER_LIGHT_POSITION_UNIFORM "light_pos_z"
#define SHADER_TEX_SET_UNIFORM "usingTex_z"
#define SHADER_MODEL_SET_UNIFORM "model_z"
#define SHADER_VIEW_SET_UNIFORM "view_z"
#define SHADER_PROJECTION_SET_UNIFORM "projection_z"
#define SHADER_VIEW_POSITION_UNIFORM "view_pos_z"

inline GLFWwindow* win;
inline Vec4 lightCol;
inline Vec3 lightPos;

inline bool frameCallbackFlag = false;
inline uint32_t frameCallbackWidth = 0;
inline uint32_t frameCallbackHeight = 0;
inline bool mouseCallbackFlag = false;
inline double mouseCallbackX = 0.0f;
inline double mouseCallbackY = 0.0f;

void initializeGL();

#endif
