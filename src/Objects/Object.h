#ifndef OBJECT_H
#define OBJECT_H

#include "Drawable.h"
#include "../Math/math.h"

#include "../Tools/ObjectLoading.h"

class Object : public Drawable {
    public:
        u32 EBO;

        Object(std::string filePath);
        //Object(std::vector<float> _verts, std::vector<u32> _indices); // TODO

        void draw(Shader& defaultShader, const Mat& viewMat, const Mat& projMat);
        void init();
};

#endif
