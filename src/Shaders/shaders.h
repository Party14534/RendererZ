#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <fstream>
#include <iterator>
#include <iostream>
#include "../../include/glad.h"
#include <GLFW/glfw3.h>

unsigned int loadShader(std::string path, int shaderType);

#endif
