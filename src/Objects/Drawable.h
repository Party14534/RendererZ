#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <vector>
#include <cstdlib>

#include "../global.h"

class Drawable {
    public:
        std::vector<Vec3> vertices;
        std::vector<u32> indices;

        size_t startingId;

        u32 VAO, VBO;

        bool initialized = false;

        Drawable(std::vector<Vec3> _vertices);
        Drawable(std::vector<Vec3> _vertices, std::vector<u32> indices);
        virtual ~Drawable();

        virtual void init() = 0;
        virtual void draw(Shader& shader) = 0;

        void setColor(Color c);
        Color getColor();

    protected:
        Color color;

};

#endif
