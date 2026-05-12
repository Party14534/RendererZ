#include "Drawable.h"

Drawable::Drawable() : transform(Mat(4, 4)) { }

Drawable::Drawable(std::vector<float> _vertices) :
    vertices(_vertices), transform(Mat(4, 4)) { }

Drawable::Drawable(std::vector<float> _vertices, std::vector<u32> _indices) :
    vertices(_vertices), indices(_indices), transform(Mat(4, 4)) { }

Drawable::~Drawable() { }

void Drawable::setColor(Color c) { 
    color = c; 
}

void Drawable::setPos(Vec3 v) { 
    pos = v; 
}

void Drawable::rotateX(float angle) { 
    float c = cos(angle);
    float s = sin(angle);

    transform.values[5]  += c;
    transform.values[6]  -= s;
    transform.values[9]  += s;
    transform.values[10] += c;
}


void Drawable::rotateY(float angle) { 
    float c = cos(angle);
    float s = sin(angle);

    transform.values[0]  += c;
    transform.values[2]  += s;
    transform.values[8]  -= s;
    transform.values[10] += c;
}

void Drawable::rotateZ(float angle) { 
    float c = cos(angle);
    float s = sin(angle);

    transform.values[0] += c;
    transform.values[1] -= s;
    transform.values[4] += s;
    transform.values[5] += c;
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
