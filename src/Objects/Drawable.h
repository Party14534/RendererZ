#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <optional>
#include <vector>
#include <cstdlib>

#include "../global.h"
#include "../Shaders/shaders.h"

#include "Texture.h"

class Drawable {
    public:
        std::vector<float> vertices;
        std::vector<u32> indices;

        size_t startingId;
        Texture* tex;

        u32 VAO, VBO;

        bool initialized = false;

        Drawable();
        Drawable(std::vector<float> _vertices);
        Drawable(std::vector<float> _vertices, std::vector<u32> indices);

        virtual ~Drawable();

        virtual void init() = 0;
        virtual void draw(Shader& shader) = 0;

        void setColor(Color c);
        void setTexture(Texture& _tex);
        Color getColor();

    protected:
        Color color;

};

#endif
