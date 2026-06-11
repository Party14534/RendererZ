#ifndef GLOBAL_H
#define GLOBAL_H

#include "../include/glad.h"

#include <GLFW/glfw3.h>
#include "Math/vec.h"
#include "Shaders/shaders.h"
#include <cstdint>
#include <iostream>

inline GLFWwindow* win;
inline Vec3 lightPos;
inline Color lightCol;
inline LightProperties lightProp;

inline bool frameCallbackFlag = false;
inline uint32_t frameCallbackWidth = 0;
inline uint32_t frameCallbackHeight = 0;
inline bool mouseCallbackFlag = false;
inline double mouseCallbackX = 0.0f;
inline double mouseCallbackY = 0.0f;

void initializeGL();

#endif
