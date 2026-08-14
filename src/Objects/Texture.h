#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <algorithm>
#include <iterator>
#include <memory>

#include "../../include/stb_image.h"
#include "../global.h"
#include "../Math/math.h"
#include "../Shaders/shaders.h"

enum TextureFilter {
    MIN_FILTER = GL_TEXTURE_MIN_FILTER,
    MAG_FILTER = GL_TEXTURE_MAG_FILTER,
    WRAP_S = GL_TEXTURE_WRAP_S,
    WRAP_T = GL_TEXTURE_WRAP_T,
    WRAP_R = GL_TEXTURE_WRAP_R
};

enum TextureFilterOption {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
    REPEAT = GL_REPEAT,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
};

enum TextureFormat {
    RED = GL_RED,
    RGB = GL_RGB,
    RGBA = GL_RGBA,
    RGBA16 = GL_RGBA16F
};

enum MipMapFilterOption {
    NEAREST_NEAREST,
    LINEAR_NEAREST,
    NEAREST_LINEAR,
    LINEAR_LINEAR
};

enum TexturePixelDataType {
    FLOAT = GL_FLOAT,
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE
};

class Texture {
    public:
        unsigned char* data;
        int width, height, nrChannels;
        std::string path;

        u32 ID;

        Color texBorderColor;

        bool loaded = false;

        Texture();
        Texture(TextureFormat internal, u32 width, u32 height, TextureFormat format, TexturePixelDataType type);
        Texture(std::string _path, bool sRGB = true, bool flipVertically = true);

        void loadImage(std::string _path, bool sRGB, bool flipVertically = true);

        void setTextureParameter(TextureFilter filter, TextureFilterOption opt);
        void attachToFramebuffer2D(u32 i);
        void setActive(u32 texNum);
        void bind();

        static std::shared_ptr<Texture> fromFile(std::string path, bool sRGB = true, bool flipVertically = true);
};

class CubeMap : public Texture {
    public:
        std::vector<std::string> fileNames;

        CubeMap(std::vector<std::string> _fileNames);
        
        void bind(u32 texNum) const;
};

#endif
