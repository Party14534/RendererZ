#include "Drawable.h"

Drawable::Drawable() { }

Drawable::Drawable(std::vector<float> _vertices) : vertices(_vertices) { }

Drawable::Drawable(std::vector<float> _vertices, std::vector<u32> _indices) :
    vertices(_vertices), indices(_indices) { }

Drawable::~Drawable() { }

void Drawable::setColor(Color c) { 
    color = c; 
}

void Drawable::setTexture(Texture& _tex) {
    tex = &_tex;
}

Color Drawable::getColor() { return color; }
