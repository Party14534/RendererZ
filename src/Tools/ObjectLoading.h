#ifndef OBJECT_LOADING_H
#define OBJECT_LOADING_H

#include "../Objects/Drawable.h"
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

shared<Mesh> LoadMeshFromFilePath(std::filesystem::path filePath);
shared<ComplexDrawable> LoadComplexDrawableFromFilePath(std::filesystem::path filePath);

#endif
