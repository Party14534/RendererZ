#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <optional>
#include <vector>
#include <cstdlib>

#include "../global.h"
#include "../Shaders/shaders.h"

#include "Texture.h"

struct VertexAttribute {
    float x, y, z, xn, yn, zn, u, v;

    VertexAttribute();
    VertexAttribute(float x, float y, float z, float xn, float yn, float zn,
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
        Material material = Material {
            Color(1.f),
            .2f,
            .5f,
            .5f,
            32.f
        };

        bool isLightSource = false;

        u32 VAO, VBO;

        bool initialized = false;

        Drawable();
        Drawable(std::vector<VertexAttribute> _vertices);
        Drawable(std::vector<VertexAttribute> _vertices, std::vector<u32> indices);

        virtual ~Drawable();

        virtual void init() = 0;
        virtual void draw(Shader& defaultShader, const Mat& viewMat, const Mat& projMat, const Vec3& viewPos) = 0;

        void setColor(Color c);
        Color getColor() const;
        void setPos(Vec3 v);
        Vec3 getPos() const;
        void setScale(Vec3 v);
        Vec3 getScale() const;
        void setMaterial(Material m);
        Material getMaterial() const;

        void rotateX(float angle);
        void rotateY(float angle);
        void rotateZ(float angle);
        void setTexture(Texture& _tex);
        void addTexture(Texture& _tex);
        void removeTexture(u32 id);
        void setShader(Shader& _shader);
        void setDefaultUniforms(Shader& shader, const Mat& viewMat, const Mat& projMat, const Vec3& viewPos);

    protected:
        Vec3 pos, rotation;
        Vec3 scale = Vec3(1.f, 1.f, 1.f);
        Mat getModelMat();
};

#endif
