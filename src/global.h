#ifndef GLOBAL_H
#define GLOBAL_H

#include <memory>

#include "System/GraphicsAPI/GraphicsApi.h"

#include "../include/stb_image.h"

inline std::shared_ptr<GraphicsAPI> api;

inline bool frameCallbackFlag = false;
inline uint32_t frameCallbackWidth = 0;
inline uint32_t frameCallbackHeight = 0;
inline bool mouseCallbackFlag = false;
inline double mouseCallbackX = 0.0f;
inline double mouseCallbackY = 0.0f;

#endif
