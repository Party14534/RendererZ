#ifndef MESH_H
#define MESH_H

#include <filesystem>
#include <vector>
#include <execution>
#include <memory>

#include "../Math/math.h"
#include "System/Buffer/Buffer.h"

struct VertexAttribute {
    float x, y, z, xn, yn, zn, u, v, xt, yt, zt, xbt, ybt, zbt;

    VertexAttribute();
    VertexAttribute(float x, float y, float z, float xn, float yn, float zn,
            float u, float v);
    VertexAttribute(float x, float y, float z, float xn, float yn, float zn,
            float u, float v, float xt, float yt, float zt,
            float xbt, float ybt, float zbt);
};

struct PointVertexAttribute {
    float x, y, z, r, g, b;

    PointVertexAttribute();
    PointVertexAttribute(float x, float y, float z, float r, float g, float b);
};

class Mesh {
    public:
        std::vector<VertexAttribute> vertices;
        std::vector<u32> indices;

        DrawType drawType = TRI;

        Mesh();
        Mesh(std::vector<VertexAttribute> verts, std::vector<u32> indices);

        Mesh(const Mesh&)            = delete;      // GL handles aren't copyable...
        Mesh& operator=(const Mesh&) = delete;
        Mesh(Mesh&&) noexcept;                      // ...but they're movable (RAII)
        Mesh& operator=(Mesh&&) noexcept;

        virtual void draw();                          // bind VAO + glDrawElements(indexCount)
        virtual void generateNormals();
        virtual void generateTangents();
        virtual void init();

        static std::shared_ptr<Mesh> tri();
        static std::shared_ptr<Mesh> plane();
        static std::shared_ptr<Mesh> cube();
        static std::shared_ptr<Mesh> skyBox();
        static std::shared_ptr<Mesh> fromOBJ(const std::filesystem::path& path,
                bool genNormals = false);

    private:
        VertexArray VAO;
        ArrayBuffer VBO;
        ElementArrayBuffer EBO;
        bool initialized = false;
};

class PointMesh : public Mesh {
    public:
        std::vector<PointVertexAttribute> points;
        u32 drawType = GL_POINTS;

        PointMesh(std::vector<PointVertexAttribute>& verts);

        PointMesh(const PointMesh&)            = delete;      // GL handles aren't copyable...
        PointMesh& operator=(const PointMesh&) = delete;
        PointMesh(PointMesh&&) noexcept;                      // ...but they're movable (RAII)
        PointMesh& operator=(PointMesh&&) noexcept;

        static std::shared_ptr<PointMesh> fromPoints(std::vector<PointVertexAttribute>& verts);

        void draw() override;                          // bind VAO + glDrawElements(indexCount)
        void init() override;
        void updateBuffer();                  // re-upload points after CPU-side mutation (e.g. physics)

    private:
        u32 VAO, VBO, EBO;
        bool initialized = false;
};

#endif
