#ifndef SKY_BOX_H
#define SKY_BOX_H

#include "Drawable.h"
#include "../Objects/Texture.h"
class SkyBox : Drawable {
    public:
        CubeMap map;

        SkyBox(std::vector<std::string> textures);
        void draw(Shader* shader, Shader& defaultShader,
                const Mat& viewMat,
                const Mat& projMat, const Vec3& viewPos,
                const DirLight& dLight,
                const std::vector<PointLight>& pLights,
                const CubeMap* cubeMap);
        void init();
    
    private:
        static const std::vector<VertexAttribute> _defaultVerts;
};

static const std::vector<VertexAttribute> _defaultVerts;

inline const std::vector<VertexAttribute> SkyBox::_defaultVerts {
    // positions          
    VertexAttribute(-1.0f,  1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(-1.0f, -1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(1.0f, -1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(1.0f, -1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(1.0f,  1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(-1.0f,  1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),

    VertexAttribute(-1.0f, -1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(-1.0f, -1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(-1.0f,  1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(-1.0f,  1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(-1.0f,  1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(-1.0f, -1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),

    VertexAttribute(1.0f, -1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(1.0f, -1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(1.0f,  1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(1.0f,  1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(1.0f,  1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(1.0f, -1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),

    VertexAttribute(-1.0f, -1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(-1.0f,  1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(1.0f,  1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(1.0f,  1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(1.0f, -1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(-1.0f, -1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),

    VertexAttribute(-1.0f,  1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(1.0f,  1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(1.0f,  1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(1.0f,  1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(-1.0f,  1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(-1.0f,  1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),

    VertexAttribute(-1.0f, -1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(-1.0f, -1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(1.0f, -1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(1.0f, -1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(-1.0f, -1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
    VertexAttribute(1.0f, -1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f),
};

#endif
