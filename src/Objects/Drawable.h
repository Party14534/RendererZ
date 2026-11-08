#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "LightSource.h"
#include <memory>
#include "../types.h"
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
    Mat4 GetModelMat();
    Mat4D GetModelMatD();
};

struct IRenderable {
      virtual void draw(shared<ShaderProgram> shader, const Mat4D& vp) = 0;
      virtual ~IRenderable() = default;
};

struct Drawable : IRenderable {
        //shared<Shader> shader = nullptr;
        shared<Mesh> mesh = nullptr;
        shared<Material> material;

        Vec2 uvScale = Vec2(1.f, 1.f);
        Transform transform;

        Drawable(shared<Mesh> m);
        Drawable(shared<Mesh> m, shared<Material> mat);

        void draw(shared<ShaderProgram> defaultShader, const Mat4D& vp);
        void drawLightPass(shared<ShaderProgram> shader);

        void setColor(Color c);
        Color getColor() const;
        void setPos(Vec3 v);
        Vec3 getPos() const;
        void setScale(Vec3 v);
        Vec3 getScale() const;
        void setMaterial(shared<Material> m);
        shared<Material> getMaterial() const;

        void rotateX(float angle);
        void rotateY(float angle);
        void rotateZ(float angle);
        void setUVScale(Vec2 scale);
        Vec2 getUVScale() const;

        // Primitives
        static Drawable Tri();
        static Drawable Plane();
        static Drawable Cube();
        static Drawable Object(std::string filePath, bool genNormals = false);
        static Drawable SkyBox(std::vector<std::string> textures);
};

struct ComplexDrawable : IRenderable {
    public:
        std::vector<Drawable> targets;

        ComplexDrawable(std::vector<Drawable>& targets);

        virtual ~ComplexDrawable();

        void draw(shared<ShaderProgram> defaultShader, const Mat4D& vp);
};

#endif
