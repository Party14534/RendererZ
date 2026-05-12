#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <string>

#include "Drawable.h"
#include "../Math/math.h"
#include "../Shaders/shaders.h"

struct Window {
    // Window variables
    u32 width, height;
    std::string windowName;

    Shader defaultShader;

    Window(u32 width, u32 height, std::string windowName);
    ~Window();

    void display();
    bool isOpen();
    void clear(Color c);
    void draw(Drawable& d);
};

#endif
