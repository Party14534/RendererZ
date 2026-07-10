#include "Drawable.h"

/*
 * Object
 */
Drawable Drawable::Object(std::string filePath, bool genNormals) {
    std::shared_ptr mesh = LoadMeshFromFilePath(filePath);
    if (genNormals) mesh->generateNormals();

    Drawable target(mesh);
    Drawable d(target);
    return d;
}

/*
 * Cube
 */
Drawable Drawable::Cube() {
    std::shared_ptr mesh = Mesh::cube();
    Drawable target(mesh);
    Drawable d(target);
    return d;
}

/*
 * Plane
 */
Drawable Drawable::Plane() {
    std::shared_ptr mesh = Mesh::plane();
    Drawable target(mesh);
    Drawable d(target);
    return d;
}

/*
 * Tri
 */
Drawable Drawable::Tri() {
    std::shared_ptr mesh = Mesh::tri();
    Drawable target(mesh);
    Drawable d(target);
    return d;
}
