#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "Drawable.h"

class Tri : public Drawable {
    public: 
        Tri(std::vector<float> _vertices);
        void draw();
};

#endif
