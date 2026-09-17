#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <algorithm>
#include <iterator>
#include <memory>
#include "../global.h"

#include "../Math/math.h"
#include "System/GraphicsAPI/GraphicsApi.h"

class Texture {
    public:
        unsigned char* data;
        int width, height, nrChannels;
        std::string path;

        u32 ID;

        Color texBorderColor;

        bool loaded = false;

        Texture();
        Texture(TextureFormat internal, u32 width, u32 height,
            TextureFormat format, DataType type, void* data);
        Texture(std::string _path, bool sRGB = true, bool flipVertically = true);

        void loadImage(std::string _path, bool sRGB, bool flipVertically = true);

        void setTextureParameter(TextureFilter filter,
                TextureFilterOption opt);
        void attachToFramebuffer2D(u32 i) const;
        void setActive(u32 texNum) const;
        void bind() const;

        static std::shared_ptr<Texture> fromFile(std::string path, bool sRGB = true, bool flipVertically = true);
};

class CubeMap : public Texture {
    public:
        std::vector<std::string> fileNames;

        CubeMap(std::vector<std::string> _fileNames);
        
        void bind(u32 texNum) const;
};

#endif
