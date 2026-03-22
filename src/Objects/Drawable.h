#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <vector>
#include <cstdlib>
#include "../../include/glad.h"
#include <GLFW/glfw3.h>

class Drawable {
    public:
        std::vector<float> vertices;
        unsigned int VBO;

        Drawable(std::vector<float> _vertices);
        virtual ~Drawable();
        virtual void draw() = 0;
};

#endif
