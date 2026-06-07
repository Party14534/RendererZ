#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "Drawable.h"

class Tri : public Drawable {
    public: 
        Tri();
        void draw(Shader& defaultShader, const Mat& viewMat, const Mat& projMat, const Vec3& viewPos);
        void init();
    
    private:
        static const std::vector<VertexAttribute> _defaultVerts;
};

inline const std::vector<VertexAttribute> Tri::_defaultVerts {
    // vertices                             // vertex normala   // texture coords
    VertexAttribute(-0.5f, -0.5f, 0.0f,     0.f, 0.f, 0.f,      0.f, 0.f),
    VertexAttribute(0.5f, -0.5f, 0.0f,      0.f, 0.f, 0.f,      1.f, 0.f),
    VertexAttribute(0.0f,  0.5f, 0.0f,      0.f, 0.f, 0.f,      0.5f, 1.f)
};


class Rect : public Drawable {
    public: 
        u32 EBO;

        Rect();
        void draw(Shader& defaultShader, const Mat& viewMat, const Mat& projMat, const Vec3& viewPos);
        void init();
    
    private:
        static const std::vector<VertexAttribute> _defaultVerts;
        static const std::vector<u32> _defaultIndices;
};

inline const std::vector<VertexAttribute> Rect::_defaultVerts {
    // vertices                             // normals          // texture coords
    VertexAttribute(0.5f, 0.5f, 0.0f,       0.f, 0.f, 0.f,      1.f, 1.f),
    VertexAttribute(0.5f, -0.5f, 0.0f,      0.f, 0.f, 0.f,      1.f, 0.f),
    VertexAttribute(-0.5f, -0.5f, 0.0f,     0.f, 0.f, 0.f,      0.f, 0.f),
    VertexAttribute(-0.5f, 0.5f, 0.0f,      0.f, 0.f, 0.f,      0.f, 1.f)
};


inline const std::vector<u32> Rect::_defaultIndices {
    0,1,3,
    1,2,3
};

class Cube : public Drawable {
    public: 
        u32 EBO;

        Cube();
        void draw(Shader& defaultShader, const Mat& viewMat, const Mat& projMat, const Vec3& viewPos);
        void init();
    
        static const std::vector<VertexAttribute> _defaultVerts;
        static const std::vector<u32> _defaultIndices;
};

// 24 vertices: 4 per face so each face carries its own full 0..1 texture
// mapping (shared corners can't, since each corner needs a different UV per
// face). Corners are ordered per face as BL(0,0) BR(1,0) TR(1,1) TL(0,1).
inline const std::vector<VertexAttribute> Cube::_defaultVerts {
    // verts                                // normals              // texture coords
    // front face (z = 0.5) - normal pointing +Z
    VertexAttribute(-0.5f, -0.5f,  0.5f,    0.f, 0.f, 1.f,  0.f, 0.f),
    VertexAttribute(0.5f, -0.5f,  0.5f,    0.f, 0.f, 1.f,  1.f, 0.f),
    VertexAttribute(0.5f,  0.5f,  0.5f,    0.f, 0.f, 1.f,  1.f, 1.f),
    VertexAttribute(-0.5f,  0.5f,  0.5f,    0.f, 0.f, 1.f,  0.f, 1.f),

    // back face (z = -0.5) - normal pointing -Z
    VertexAttribute(0.5f, -0.5f, -0.5f,    0.f, 0.f, -1.f,  0.f, 0.f),
    VertexAttribute(-0.5f, -0.5f, -0.5f,    0.f, 0.f, -1.f,  1.f, 0.f),
    VertexAttribute(-0.5f,  0.5f, -0.5f,    0.f, 0.f, -1.f,  1.f, 1.f),
    VertexAttribute(0.5f,  0.5f, -0.5f,    0.f, 0.f, -1.f,  0.f, 1.f),

    // left face (x = -0.5) - normal pointing -X
    VertexAttribute(-0.5f, -0.5f, -0.5f,    -1.f, 0.f, 0.f,  0.f, 0.f),
    VertexAttribute(-0.5f, -0.5f,  0.5f,    -1.f, 0.f, 0.f,  1.f, 0.f),
    VertexAttribute(-0.5f,  0.5f,  0.5f,    -1.f, 0.f, 0.f,  1.f, 1.f),
    VertexAttribute(-0.5f,  0.5f, -0.5f,    -1.f, 0.f, 0.f,  0.f, 1.f),

    // right face (x = 0.5) - normal pointing +X
    VertexAttribute(0.5f, -0.5f,  0.5f,    1.f, 0.f, 0.f,  0.f, 0.f),
    VertexAttribute(0.5f, -0.5f, -0.5f,    1.f, 0.f, 0.f,  1.f, 0.f),
    VertexAttribute(0.5f,  0.5f, -0.5f,    1.f, 0.f, 0.f,  1.f, 1.f),
    VertexAttribute(0.5f,  0.5f,  0.5f,    1.f, 0.f, 0.f,  0.f, 1.f),

    // bottom face (y = -0.5) - normal pointing -Y
    VertexAttribute(-0.5f, -0.5f, -0.5f,    0.f, -1.f, 0.f,  0.f, 0.f),
    VertexAttribute(0.5f, -0.5f, -0.5f,    0.f, -1.f, 0.f,  1.f, 0.f),
    VertexAttribute(0.5f, -0.5f,  0.5f,    0.f, -1.f, 0.f,  1.f, 1.f),
    VertexAttribute(-0.5f, -0.5f,  0.5f,    0.f, -1.f, 0.f,  0.f, 1.f),

    // top face (y = 0.5) - normal pointing +Y
    VertexAttribute(-0.5f,  0.5f,  0.5f,    0.f, 1.f, 0.f,  0.f, 0.f),
    VertexAttribute(0.5f,  0.5f,  0.5f,    0.f, 1.f, 0.f,  1.f, 0.f),
    VertexAttribute(0.5f,  0.5f, -0.5f,    0.f, 1.f, 0.f,  1.f, 1.f),
    VertexAttribute(-0.5f,  0.5f, -0.5f,    0.f, 1.f, 0.f,  0.f, 1.f)
};

// Two triangles per face: BL,BR,TR and TR,TL,BL (base = face * 4).
inline const std::vector<u32> Cube::_defaultIndices {
     0,  1,  2,   2,  3,  0,   // front
     4,  5,  6,   6,  7,  4,   // back
     8,  9, 10,  10, 11,  8,   // left
    12, 13, 14,  14, 15, 12,   // right
    16, 17, 18,  18, 19, 16,   // bottom
    20, 21, 22,  22, 23, 20    // top
};

#endif
