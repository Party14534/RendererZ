#ifndef SCENE_H
#define SCENE_H

#include <list>
#include <memory>

#include "Drawable.h"
#include "../Shaders/shaders.h"

struct Scene {
    std::list<Drawable*> sceneObjects;

    Scene();
    Scene(std::initializer_list<Drawable*> objects);

    void addToList(Drawable* object);

    void draw(std::shared_ptr<Shader> defaultShader);
};

#endif
