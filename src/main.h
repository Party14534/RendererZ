#ifndef MAIN_H
#define MAIN_H

#include "Objects/Scene.h"
#include "System/Window.h"
#include "Objects/Primitives.h"
#include "Shaders/shaders.h"
#include "Input/Input.h"


void processInput(Window& win);
std::vector<PointVertexAttribute> makeTestPointCloud();
void applyGravity(std::vector<PointVertexAttribute>& points);

#endif
