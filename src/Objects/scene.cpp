#include "Scene.h"

Scene::Scene() { }

Scene::Scene(std::initializer_list<Drawable*> objects) {
    sceneObjects = objects;
}

void Scene::addToList(Drawable* object) {
    sceneObjects.push_back(object);
}

void Scene::draw(std::shared_ptr<Shader> defaultShader) {
    for(auto& o : sceneObjects) {
        o->draw(defaultShader);
    }
}
