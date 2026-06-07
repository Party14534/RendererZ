#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <optional>
#include <vector>
#include <cstdlib>

#include "../global.h"
#include "../Shaders/shaders.h"

#include "Texture.h"

struct VertexAttribute {
    float x, y, z, r, g, b, u, v;

    VertexAttribute();
    VertexAttribute(float x, float y, float z, float r, float g, float b,
            float u, float v);
};

class Drawable {
    public:
        std::vector<VertexAttribute> vertices;
        std::vector<u32> indices;

        Mat modelMat;

        size_t startingId;
        std::vector<Texture*> texs;
        Shader* shader = nullptr;

        bool isLightSource = false;

        u32 VAO, VBO;

        bool initialized = false;

        Drawable();
        Drawable(std::vector<VertexAttribute> _vertices);
        Drawable(std::vector<VertexAttribute> _vertices, std::vector<u32> indices);

        virtual ~Drawable();

        virtual void init() = 0;
        virtual void draw(Shader& defaultShader, const Mat& viewMat, const Mat& projMat) = 0;

        void setColor(Color c);
        Color getColor();
        void setPos(Vec3 v);
        Vec3 getPos();
        void setScale(Vec3 v);
        Vec3 getScale();

        void rotateX(float angle);
        void rotateY(float angle);
        void rotateZ(float angle);
        void setTexture(Texture& _tex);
        void addTexture(Texture& _tex);
        void removeTexture(u32 id);
        void setShader(Shader& _shader);

    protected:
        Color color;
        Vec3 pos, rotation;
        Vec3 scale = Vec3(1.f, 1.f, 1.f);
        Mat getModelMat();
};

#endif
