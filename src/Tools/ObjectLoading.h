#ifndef OBJECT_LOADING_H
#define OBJECT_LOADING_H

#include "../Objects/Mesh.h"
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>

std::shared_ptr<Mesh> LoadMeshFromFilePath(std::filesystem::path filePath);

#endif
