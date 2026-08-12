#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "LightSource.h"
#include <memory>
#include <optional>
#include <vector>
#include <cstdlib>

#include "../global.h"
#include "../Shaders/Shaders.h"
#include "Mesh.h"
#include "Texture.h"

struct Transform {
    Vec3 pos, rotation;
    Vec3 scale = Vec3(1.f);
    Mat GetModelMat();
};

struct Drawable {
        std::shared_ptr<Shader> shader = nullptr;
        std::shared_ptr<Mesh> mesh = nullptr;
        Material material = Material {
            Color(1.f),
            .2f,
            .5f,
            .5f,
            32.f
        };
        std::shared_ptr<Texture> diffuseTexture;
        std::shared_ptr<Texture> normalTexture;
        Vec2 uvScale = Vec2(1.f, 1.f);
        Transform transform;

        Drawable(std::shared_ptr<Mesh> m);

        void draw(std::shared_ptr<Shader> defaultShader);

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
        void setDiffuseTexture(std::shared_ptr<Texture> _tex);
        void setNormalTexture(std::shared_ptr<Texture> _tex);
        void setShader(std::shared_ptr<Shader> _shader);
        void setUVScale(Vec2 scale);
        Vec2 getUVScale() const;

        // Primitives
        static Drawable Tri();
        static Drawable Plane();
        static Drawable Cube();
        static Drawable Object(std::string filePath, bool genNormals = false);
        static Drawable SkyBox(std::vector<std::string> textures);
};

struct ComplexDrawable {
    public:
        std::vector<Drawable> targets;

        ComplexDrawable(std::vector<Drawable>& targets);

        virtual ~ComplexDrawable();

        void draw(std::shared_ptr<Shader> defaultShader);
};

#endif
