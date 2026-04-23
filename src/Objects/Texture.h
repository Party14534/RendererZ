#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include "../../include/stb_image.h"
#include "../global.h"
#include "Types.h"

enum TextureParameterOption {
    REPEAT,
    MIRRORED_REPEAT,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER
};

enum TextureFilterOption {
    NEAREST,
    LINEAR
};

enum MipMapFilterOption {
    NEAREST_NEAREST,
    LINEAR_NEAREST,
    NEAREST_LINEAR,
    LINEAR_LINEAR
};

struct Texture {
    unsigned char* data;
    int width, height, nrChannels;
    std::string path;

    u32 ID;

    TextureParameterOption texParameter = TextureParameterOption::REPEAT;
    TextureFilterOption texFilter = TextureFilterOption::NEAREST;
    MipMapFilterOption mmFilter = MipMapFilterOption::NEAREST_NEAREST;
    Color texBorderColor;

    bool loaded = false;

    Texture();
    Texture(std::string _path);

    void loadImage(std::string _path);

    void setTextureParameter(TextureParameterOption _texParameter);
    void setTextureFilter(TextureFilterOption _texFilter);
    void setMipMapFilter(MipMapFilterOption _mmFilter);

    void bind(u32 texNum);
};

#endif
