#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "Drawable.h"

class Tri : public Drawable {
    public: 
        Tri();
        void draw(Shader& defaultShader, const Mat& viewMat, const Mat& projMat);
        void init();
    
    private:
        static const std::vector<float> _defaultVerts;
};

inline const std::vector<float> Tri::_defaultVerts {
    // vertices             // colors           // texture coords
    -0.5f, -0.5f, 0.0f,     0.f, 0.f, 0.f,      0.f, 0.f,
    0.5f, -0.5f, 0.0f,      0.f, 0.f, 0.f,      1.f, 0.f,
    0.0f,  0.5f, 0.0f,      0.f, 0.f, 0.f,      0.5f, 1.f
};


class Rect : public Drawable {
    public: 
        u32 EBO;

        Rect();
        void draw(Shader& defaultShader, const Mat& viewMat, const Mat& projMat);
        void init();
    
    private:
        static const std::vector<float> _defaultVerts;
        static const std::vector<u32> _defaultIndices;
};

inline const std::vector<float> Rect::_defaultVerts {
    // vertices             // colors           // texture coords
    0.5f, 0.5f, 0.0f,       0.f, 0.f, 0.f,      1.f, 1.f,
    0.5f, -0.5f, 0.0f,      0.f, 0.f, 0.f,      1.f, 0.f,
    -0.5f, -0.5f, 0.0f,     0.f, 0.f, 0.f,      0.f, 0.f,
    -0.5f, 0.5f, 0.0f,      0.f, 0.f, 0.f,      0.f, 1.f
};


inline const std::vector<u32> Rect::_defaultIndices {
    0,1,3,
    1,2,3
};

class Cube : public Drawable {
    public: 
        u32 EBO;

        Cube();
        void draw(Shader& defaultShader, const Mat& viewMat, const Mat& projMat);
        void init();
    
    private:
        static const std::vector<float> _defaultVerts;
        static const std::vector<u32> _defaultIndices;
};

inline const std::vector<float> Cube::_defaultVerts {
    // verts                // colors       // texture coords
    -0.5f, -0.5f, -0.5f,    0.f, 0.f, 0.f,  0.f, 0.f,
     0.5f, -0.5f, -0.5f,    0.f, 0.f, 0.f,  1.f, 0.f,
     0.5f,  0.5f, -0.5f,    0.f, 0.f, 0.f,  1.f, 1.f,
    -0.5f,  0.5f, -0.5f,    0.f, 0.f, 0.f,  0.f, 1.f,

    -0.5f, -0.5f,  0.5f,    0.f, 0.f, 0.f,  0.f, 0.f,
     0.5f, -0.5f,  0.5f,    0.f, 0.f, 0.f,  1.f, 0.f,
     0.5f,  0.5f,  0.5f,    0.f, 0.f, 0.f,  1.f, 1.f,
    -0.5f,  0.5f,  0.5f,    0.f, 0.f, 0.f,  0.f, 1.f,
};

inline const std::vector<u32> Cube::_defaultIndices {
    0, 1, 3, 3, 1, 2,
    1, 5, 2, 2, 5, 6,
    5, 4, 6, 6, 4, 7,
    4, 0, 7, 7, 0, 3,
    3, 2, 7, 7, 2, 6,
    4, 5, 0, 0, 5, 1
};

#endif
