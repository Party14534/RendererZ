#include "Mesh.h"
#include "Primitives.h"
#include "../Tools/ObjectLoading.h"
#include "global.h"
#include <execution>
#include <memory>

Mesh::Mesh(std::vector<VertexAttribute> vertices, std::vector<u32> indices) :
    vertices(vertices),
    indices(indices)
{ }

Mesh::Mesh() { }

void Mesh::init() {
    // Generate VAO and VBO and EBO
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    // Bind and set up buffers
    glBindVertexArray(VAO); // MUST BIND VAO FIRST

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexAttribute),
            vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32),
            indices.data(), GL_STATIC_DRAW);

    // Set attribs
    // vertex pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                            14 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);

    // vertex normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                            14 * sizeof(float),
                            (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // tex coord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                            14 * sizeof(float),
                            (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
                            14 * sizeof(float),
                            (void*)(8*sizeof(float)));
    glEnableVertexAttribArray(3);

    // bitangent
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE,
                            14 * sizeof(float),
                            (void*)(11*sizeof(float)));
    glEnableVertexAttribArray(4);

    initialized = true;
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

std::shared_ptr<Mesh> Mesh::tri() {
    std::shared_ptr<Mesh> m = std::make_shared<Mesh>(triDefaultVerts, triDefaultIndices);
    m->generateTangents();
    return m;
}

std::shared_ptr<Mesh> Mesh::plane() {
    std::shared_ptr<Mesh> m = std::make_shared<Mesh>(planeDefaultVerts, planeDefaultIndices);
    m->generateTangents();
    return m;
}

std::shared_ptr<Mesh> Mesh::cube() {
    std::shared_ptr<Mesh> m = std::make_shared<Mesh>(cubeDefaultVerts, cubeDefaultIndices);
    m->generateTangents();
    return m;
}

std::shared_ptr<Mesh> Mesh::skyBox() {
    std::shared_ptr<Mesh> m = std::make_shared<Mesh>(skyBoxDefaultVerts, skyBoxDefaultIndices);
    return m;
}

std::shared_ptr<Mesh> Mesh::fromOBJ(const std::filesystem::path &path, bool genNormals) {
    std::shared_ptr<Mesh> m = LoadMeshFromFilePath(path);
    if (genNormals) {
        m->generateNormals();
        m->generateTangents();
    }
    return m;
}

void Mesh::generateNormals() {
    std::for_each(
        std::execution::par,
        vertices.begin(),
        vertices.end(),
        [this](VertexAttribute& vert) {
            u32 index = &vert - &this->vertices[0];
            std::vector<u32> vertIndices;
            Vec3 normal;

            // Get the surrounding faces
            for (u32 i = 0; i < indices.size(); i += 3) {
                if (indices[i] == index ||
                        indices[i+1] == index || 
                        indices[i+2] == index) {
                    vertIndices.push_back(indices[i]);
                    vertIndices.push_back(indices[i+1]);
                    vertIndices.push_back(indices[i+2]);
                }
            }

            // Sum up surrounding normals
            for (u32 i = 0; i < vertIndices.size(); i += 3) {
                VertexAttribute attr1 = vertices[vertIndices[i]];
                VertexAttribute attr2 = vertices[vertIndices[i+1]];
                VertexAttribute attr3 = vertices[vertIndices[i+2]];
                Vec3 x = Vec3(attr1.x, attr1.y, attr1.z);
                Vec3 y = Vec3(attr2.x, attr2.y, attr2.z);
                Vec3 z = Vec3(attr3.x, attr3.y, attr3.z);
                normal = normal + CalculateFaceNormal(x, y, z);
            }

            normal = normal.normalize();
            vertices[index].xn = normal.x;
            vertices[index].yn = normal.y;
            vertices[index].zn = normal.z;
        }
    );
}

void Mesh::generateTangents() {
    for (VertexAttribute& v : vertices) {
        v.xt = v.yt = v.zt = 0.f;
        v.xbt = v.ybt = v.zbt = 0.f;
    }

    for (u32 i = 0; i + 2 < indices.size(); i += 3) {
        VertexAttribute& v0 = vertices[indices[i]];
        VertexAttribute& v1 = vertices[indices[i+1]];
        VertexAttribute& v2 = vertices[indices[i+2]];

        Vec3 edge1(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        Vec3 edge2(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
        float du1 = v1.u - v0.u, dv1 = v1.v - v0.v;
        float du2 = v2.u - v0.u, dv2 = v2.v - v0.v;

        float det = du1 * dv2 - du2 * dv1;
        if (det > -1e-8f && det < 1e-8f) continue;
        float f = 1.f / det;

        Vec3 tangent = edge1 * (dv2 * f) - edge2 * (dv1 * f);
        Vec3 bitangent = edge2 * (du1 * f) - edge1 * (du2 * f);

        for (VertexAttribute* v : { &v0, &v1, &v2 }) {
            v->xt += tangent.x; v->yt += tangent.y; v->zt += tangent.z;
            v->xbt += bitangent.x; v->ybt += bitangent.y; v->zbt += bitangent.z;
        }
    }

    for (VertexAttribute& v : vertices) {
        Vec3 t(v.xt, v.yt, v.zt);
        Vec3 b(v.xbt, v.ybt, v.zbt);
        if (t.length() > 1e-8f) t = t.normalize();
        if (b.length() > 1e-8f) b = b.normalize();
        v.xt = t.x; v.yt = t.y; v.zt = t.z;
        v.xbt = b.x; v.ybt = b.y; v.zbt = b.z;
    }
}

void Mesh::draw() {
    if (!initialized) init();
    glBindVertexArray(VAO);
    glDrawElements(drawType, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


/*
 * Point Mesh
 */
PointMesh::PointMesh(std::vector<PointVertexAttribute>& _points) :
    points(_points)
{ }

std::shared_ptr<PointMesh> PointMesh::fromPoints(std::vector<PointVertexAttribute> &points) {
    return std::make_shared<PointMesh>(PointMesh(points));
}

PointMesh::PointMesh(PointMesh&& other) noexcept :
    points(std::move(other.points)),
    VAO(other.VAO), VBO(other.VBO), EBO(other.EBO),
    initialized(other.initialized)
{
    other.VAO = other.VBO = other.EBO = 0;
    other.initialized = false;
}

PointMesh& PointMesh::operator=(PointMesh&& other) noexcept {
    if (this != &other) {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);

        points = std::move(other.points);
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
        initialized = other.initialized;

        other.VAO = other.VBO = other.EBO = 0;
        other.initialized = false;
    }
    return *this;
}

void PointMesh::init() {
    // Generate VAO and VBO
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    // Bind and set up buffers
    glBindVertexArray(VAO); // MUST BIND VAO FIRST

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(PointVertexAttribute),
            points.data(), GL_DYNAMIC_DRAW);

    // Set attribs
    // point pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                            6 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);

    // point color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                            6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    initialized = true;
}

PointMesh::~PointMesh() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void PointMesh::draw() {
    if (!initialized) init();
    glBindVertexArray(VAO);
    glDrawArrays(drawType, 0, points.size());
    glBindVertexArray(0);
}

void PointMesh::updateBuffer() {
    if (!initialized) { init(); return; }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
            points.size() * sizeof(PointVertexAttribute), points.data());
}

/*
 * Helper types
 */

VertexAttribute::VertexAttribute()
    : x(0), y(0), z(0), xn(0), yn(0), zn(0), u(0), v(0),
      xt(0), yt(0), zt(0), xbt(0), ybt(0), zbt(0) {}

VertexAttribute::VertexAttribute(
        float x, float y, float z,
        float xn,float yn,float zn,
        float u, float v)
    : x(x), y(y), z(z), xn(xn), yn(yn), zn(zn), u(u), v(v),
      xt(0), yt(0), zt(0), xbt(0), ybt(0), zbt(0) {}

VertexAttribute::VertexAttribute(
        float x, float y, float z,
        float xn, float yn, float zn,
        float u, float v,
        float xt, float yt, float zt,
        float xbt, float ybt, float zbt)
    : x(x), y(y), z(z), xn(xn), yn(yn), zn(zn), u(u), v(v),
      xt(xt), yt(yt), zt(zt), xbt(xbt), ybt(ybt), zbt(zbt) {}

PointVertexAttribute::PointVertexAttribute() 
    : x(0), y(0), z(0), r(0), g(0), b(0) {}

PointVertexAttribute::PointVertexAttribute(
        float x, float y, float z, float r, float g, float b)
    : x(x), y(y), z(z), r(r), g(g), b(b) {}

