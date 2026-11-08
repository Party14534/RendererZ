#include "Drawable.h"
#include "../Tools/ObjectLoading.h"

/*
 * Object
 */
Drawable Drawable::Object(std::string filePath, bool genNormals) {
    shared<Mesh> mesh = LoadMeshFromFilePath(filePath);
    if (genNormals) mesh->generateNormals();

    Drawable target(mesh);
    Drawable d(target);
    return d;
}

/*
 * Cube
 */
Drawable Drawable::Cube() {
    shared<Mesh> mesh = Mesh::cube();
    Drawable target(mesh);
    Drawable d(target);
    return d;
}

/*
 * Plane
 */
Drawable Drawable::Plane() {
    shared<Mesh> mesh = Mesh::plane();
    Drawable target(mesh);
    Drawable d(target);
    return d;
}

/*
 * Tri
 */
Drawable Drawable::Tri() {
    shared<Mesh> mesh = Mesh::tri();
    Drawable target(mesh);
    Drawable d(target);
    return d;
}
