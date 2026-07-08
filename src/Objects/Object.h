#ifndef OBJECT_H
#define OBJECT_H

#include <algorithm>
#include <execution>
#include <iterator>

#include "Drawable.h"
#include "../Math/math.h"
#include "../Tools/ObjectLoading.h"

class Object : public Drawable {
    public:
        u32 EBO;

        Object(std::string filePath, bool genNormals = false);
        //Object(std::vector<float> _verts, std::vector<u32> _indices); // TODO

        void draw(Shader* shader, Shader& defaultShader,
                const Mat& viewMat,
                const Mat& projMat, const Vec3& viewPos,
                const DirLight& dLight,
                const std::vector<PointLight>& pLights,
                const CubeMap* skyBox);
        void init();

        void generateNormals();
};

#endif
