#ifndef MAIN_H
#define MAIN_H

#include "System/Window.h"
#include "Objects/Primitives.h"
#include "Shaders/Shaders.h"
#include "Input/Input.h"


void processInput(Window& win);
std::vector<PointVertexAttribute> makeTestPointCloud();
void applyGravity(std::vector<PointVertexAttribute>& points);

#endif
