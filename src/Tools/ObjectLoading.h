#ifndef OBJECT_LOADING_H
#define OBJECT_LOADING_H

#include "Objects/Drawable.h"
#include <filesystem>
#include <string>

void LoadObjectFromFilePath(Drawable* d, std::filesystem::path filePath);

#endif
