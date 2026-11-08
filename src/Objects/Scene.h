#ifndef SCENE_H
#define SCENE_H

#include <list>
#include <memory>
#include "../types.h"

#include "Drawable.h"
#include "../Shaders/Shaders.h"

struct Scene {
    std::list<IRenderable*> sceneObjects;

    Scene();
    Scene(std::initializer_list<IRenderable*> objects);

    void addToList(IRenderable* object);
    void addToList(std::initializer_list<IRenderable*> objects);

    void draw(shared<ShaderProgram> defaultShader, const Mat4D& vp);
};

#endif
