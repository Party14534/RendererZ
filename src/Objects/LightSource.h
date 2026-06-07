#ifndef LIGHT_SOURCE
#define LIGHT_SOURCE

#include "Drawable.h"

class LightSource : public Drawable {
    public:
        u32 EBO;

        LightSource();
        LightSource(std::vector<VertexAttribute> verts, std::vector<u32> indices);

        void draw(Shader& defaultShader, const Mat& viewMat, const Mat& projMat, const Vec3& viewPos);
        void init(); 
};

#endif
