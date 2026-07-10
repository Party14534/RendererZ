#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "Drawable.h"

inline const std::vector<VertexAttribute> triDefaultVerts {
    // vertices                             // vertex normala   // texture coords
    VertexAttribute(-0.5f, -0.5f, 0.0f,     0.f, 0.f, 0.f,      0.f, 0.f),
    VertexAttribute(0.5f, -0.5f, 0.0f,      0.f, 0.f, 0.f,      1.f, 0.f),
    VertexAttribute(0.0f,  0.5f, 0.0f,      0.f, 0.f, 0.f,      0.5f, 1.f)
};

inline const std::vector<u32> triDefaultIndices {
    0,1,2
};

inline const std::vector<VertexAttribute> planeDefaultVerts {
    // vertices                             // normals          // texture coords
    VertexAttribute(0.5f, 0.5f, 0.0f,       0.f, 0.f, 0.f,      1.f, 1.f),
    VertexAttribute(0.5f, -0.5f, 0.0f,      0.f, 0.f, 0.f,      1.f, 0.f),
    VertexAttribute(-0.5f, -0.5f, 0.0f,     0.f, 0.f, 0.f,      0.f, 0.f),
    VertexAttribute(-0.5f, 0.5f, 0.0f,      0.f, 0.f, 0.f,      0.f, 1.f)
};


inline const std::vector<u32> planeDefaultIndices {
    0,1,3,
    1,2,3
};

// 24 vertices: 4 per face so each face carries its own full 0..1 texture
// mapping (shared corners can't, since each corner needs a different UV per
// face). Corners are ordered per face as BL(0,0) BR(1,0) TR(1,1) TL(0,1).
inline const std::vector<VertexAttribute> cubeDefaultVerts {
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
inline const std::vector<u32> cubeDefaultIndices {
     0,  1,  2,   2,  3,  0,   // front
     4,  5,  6,   6,  7,  4,   // back
     8,  9, 10,  10, 11,  8,   // left
    12, 13, 14,  14, 15, 12,   // right
    16, 17, 18,  18, 19, 16,   // bottom
    20, 21, 22,  22, 23, 20    // top
};

inline const std::vector<VertexAttribute> skyBoxDefaultVerts {
    // positions
    VertexAttribute(-1.0f,  1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f), // 0
    VertexAttribute(-1.0f, -1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f), // 1
    VertexAttribute( 1.0f, -1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f), // 2
    VertexAttribute( 1.0f,  1.0f, -1.0f, 0.f, 0.f, 0.f, 0.f, 0.f), // 3
    VertexAttribute(-1.0f, -1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f), // 4
    VertexAttribute(-1.0f,  1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f), // 5
    VertexAttribute( 1.0f,  1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f), // 6
    VertexAttribute( 1.0f, -1.0f,  1.0f, 0.f, 0.f, 0.f, 0.f, 0.f), // 7
};

inline const std::vector<u32> skyBoxDefaultIndices {
    0, 1, 2,  2, 3, 0, // back
    4, 1, 0,  0, 5, 4, // left
    2, 7, 6,  6, 3, 2, // right
    4, 5, 6,  6, 7, 4, // front
    0, 3, 6,  6, 5, 0, // top
    1, 4, 2,  2, 4, 7, // bottom
};

#endif
