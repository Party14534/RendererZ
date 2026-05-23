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

        Mat transform;

        size_t startingId;
        std::vector<Texture*> texs;
        Shader* shader;

        u32 VAO, VBO;

        bool initialized = false;

        Drawable();
        Drawable(std::vector<float> _vertices);
        Drawable(std::vector<float> _vertices, std::vector<u32> indices);

        virtual ~Drawable();

        virtual void init() = 0;
        virtual void draw(Shader& shader) = 0;

        void setColor(Color c);
        void setPos(Vec3 v);
        void rotateX(float angle);
        void rotateY(float angle);
        void rotateZ(float angle);
        void setScale(Vec3 v);
        void setTexture(Texture& _tex);
        void addTexture(Texture& _tex);
        void removeTexture(u32 id);
        void setShader(Shader& _shader);
        Color getColor();

    protected:
        Color color;
        Vec3 pos, rotation, scale;
        Mat getTransMat();
};

#endif
