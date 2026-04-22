#include "Texture.h"

Texture::Texture() { }

Texture::Texture(std::string _path) : path(_path) {
    loadImage(_path);
}

void Texture::loadImage(std::string _path) {
    path = _path;
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 3);

    if (data == nullptr) { 
        std::cout << "Failed to load image " << _path << "\n";
        return;
    }

    glGenTextures(1, &ID);

    glBindTexture(GL_TEXTURE_2D, ID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
            0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    loaded = true;
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::setTextureParameter(TextureParameterOption _texParameter) {
    texParameter = _texParameter;
}

void Texture::setTextureFilter(TextureFilterOption _texFilter) {
    texFilter = _texFilter;
}
void Texture::setMipMapFilter(MipMapFilterOption _mmFilter) {
    mmFilter = _mmFilter;
}
