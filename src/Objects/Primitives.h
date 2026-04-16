#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "Drawable.h"

class Tri : public Drawable {
    public: 
        Tri(std::vector<Vec3> _vertices);
        void draw();
        void init();
};

class Rect : public Drawable {
    public: 
        u32 EBO;

        Rect(std::vector<Vec3> _vertices, std::vector<u32> _indices);
        void draw();
        void init();
};

#endif
