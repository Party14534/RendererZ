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
    if (texs.size() == 0) {
        texs.push_back(&_tex);
    } else {
        texs[0] = &_tex;
    }
}

void Drawable::addTexture(Texture& _tex) {
    texs.push_back(&_tex);
}

void Drawable::setShader(Shader& _shader) {
    shader = &_shader;
}

Color Drawable::getColor() { return color; }
