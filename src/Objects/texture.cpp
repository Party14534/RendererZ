#include "Texture.h"
#include "../Shaders/shaders.h"
#include "global.h"
#include <algorithm>
#include <iterator>
#include <memory>

Texture::Texture() { }

Texture::Texture(TextureFormat internal, u32 width, u32 height,
        TextureFormat format, TexturePixelDataType type) { 
    glGenTextures(1, &ID);
    loaded = true;
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, format, type, 0);
}

Texture::Texture(std::string _path, bool sRGB, bool flipVertically) : path(_path) {
    loadImage(_path, sRGB, flipVertically);
}

std::shared_ptr<Texture> Texture::fromFile(std::string path, bool sRGB, bool flipVertically) {
    Texture t(path, sRGB, flipVertically);
    return std::make_shared<Texture>(t);
}

void Texture::loadImage(std::string _path, bool sRGB, bool flipVertically) {
    path = _path;
    stbi_set_flip_vertically_on_load(flipVertically);
    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 3);

    if (data == nullptr) { 
        std::cout << "Failed to load image " << _path << "\n";
        return;
    }

    glGenTextures(1, &ID);

    glBindTexture(GL_TEXTURE_2D, ID);

    u32 format = (sRGB) ? GL_SRGB : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
            0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    loaded = true;
}

void Texture::bind() {
    if(!loaded) { std::cerr << "Binding unloaded texture\n"; exit(1); }
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::setActive(u32 texNum) {
    if(!loaded) { std::cerr << "Binding unloaded texture\n"; exit(1); }
    glActiveTexture(GL_TEXTURE0 + texNum);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::setTextureParameter(TextureFilter filter, TextureFilterOption opt) {
    glTexParameteri(GL_TEXTURE_2D, filter, opt);
}

void Texture::attachToFramebuffer2D(u32 i) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, ID, 0);
}


/*
 * Cube Map
 */

CubeMap::CubeMap(std::vector<std::string> _fileNames) {
    std::copy(_fileNames.begin(), _fileNames.end(), std::back_inserter(fileNames));
    stbi_set_flip_vertically_on_load(false);

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    int width, height, nrChannels;
    for (u32 i = 0; i < fileNames.size(); i++) {
        unsigned char *data = stbi_load(fileNames[i].c_str(), &width, &height, &nrChannels, 0);
        if (data == nullptr) {
            std::cout << "Cubemap tex failed to load at path: " << _fileNames[i] << std::endl;
            stbi_image_free(data);
            return;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB,
                width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CubeMap::bind(u32 texNum) const {
    glActiveTexture(GL_TEXTURE0 + texNum);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}
