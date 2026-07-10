#ifndef MESH_H
#define MESH_H

#include <filesystem>
#include <vector>

#include "../global.h"
#include "Math/math.h"

struct VertexAttribute {
    float x, y, z, xn, yn, zn, u, v;

    VertexAttribute();
    VertexAttribute(float x, float y, float z, float xn, float yn, float zn,
            float u, float v);
};

class Mesh {
    public:
        std::vector<VertexAttribute> vertices;
        std::vector<u32> indices;

        Mesh(std::vector<VertexAttribute> verts, std::vector<u32> indices);
        ~Mesh();                                   // glDeleteBuffers / glDeleteVertexArrays

        Mesh(const Mesh&)            = delete;      // GL handles aren't copyable...
        Mesh& operator=(const Mesh&) = delete;
        Mesh(Mesh&&) noexcept;                      // ...but they're movable (RAII)
        Mesh& operator=(Mesh&&) noexcept;

        void draw();                          // bind VAO + glDrawElements(indexCount)
        void generateNormals();
        void init();

        static std::shared_ptr<Mesh> tri();
        static std::shared_ptr<Mesh> plane();
        static std::shared_ptr<Mesh> cube();
        static std::shared_ptr<Mesh> skyBox();
        static std::shared_ptr<Mesh> fromOBJ(const std::filesystem::path& path,
                bool genNormals = false);

    private:
        u32 VAO, VBO, EBO;
        bool initialized = false;
};

#endif
