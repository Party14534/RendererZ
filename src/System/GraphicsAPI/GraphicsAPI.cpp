#include "GraphicsAPI.h"

GraphicsAPI::GraphicsAPI(
        const u32 width, 
        const u32 height, 
        const std::string& windowName
    ) : 
    width(width),
    height(height),
    windowName(windowName) { }
