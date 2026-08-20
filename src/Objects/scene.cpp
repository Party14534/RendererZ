#include "Scene.h"

Scene::Scene() { }

Scene::Scene(std::initializer_list<IRenderable*> objects) {
    sceneObjects = objects;
}

void Scene::addToList(IRenderable* object) {
    sceneObjects.push_back(object);
}

void Scene::addToList(std::initializer_list<IRenderable*> objects) {
    sceneObjects.insert(sceneObjects.end(), objects);
}

void Scene::draw(std::shared_ptr<ShaderProgram> defaultShader, const Mat4D& vp) {
    for(auto& o : sceneObjects) {
        o->draw(defaultShader, vp);
    }
}
