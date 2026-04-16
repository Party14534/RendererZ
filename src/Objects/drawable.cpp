#include "Drawable.h"

Drawable::Drawable(std::vector<Vec3> _vertices) : vertices(_vertices) { }

Drawable::Drawable(std::vector<Vec3> _vertices, std::vector<u32> _indices) : vertices(_vertices), indices(_indices) { }

Drawable::~Drawable() { }

void Drawable::setColor(Color c) { 
    color = c; 
}

Color Drawable::getColor() { return color; }
