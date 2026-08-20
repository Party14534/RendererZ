#ifndef SCENE_H
#define SCENE_H

#include <list>
#include <memory>

#include "Drawable.h"
#include "../Shaders/shaders.h"

struct Scene {
    std::list<IRenderable*> sceneObjects;

    Scene();
    Scene(std::initializer_list<IRenderable*> objects);

    void addToList(IRenderable* object);
    void addToList(std::initializer_list<IRenderable*> objects);

    void draw(std::shared_ptr<ShaderProgram> defaultShader, const Mat4D& vp);
};

#endif
