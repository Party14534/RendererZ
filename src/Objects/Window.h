#ifndef WINDOW_H
#define WINDOW_H

#include <string>

#include "Drawable.h"
#include "../Math/math.h"
#include "../Shaders/shaders.h"

struct Window {
    // Window variables
    u32 width, height;
    std::string windowName;

    Shader defaultShader;

    Mat orthoMat;
    Mat perspMat;
    Mat viewMat;
    float aspectRatio;
    float fov;
    bool isOrthographic = false;

    Window(u32 width, u32 height, std::string windowName);
    ~Window();

    void display();
    bool isOpen();
    void clear(Color c);
    void draw(Drawable& d);

    void buildPerspectiveMatrices();
};

#endif
