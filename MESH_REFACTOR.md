# Mesh Abstraction — Refactor Plan

This covers the roadmap's open Milestone 0 item: **a real `Mesh` abstraction**. It
describes the problems with the current `Drawable` hierarchy, what a proper `Mesh`
class looks like, and a migration plan to get there without breaking the build
mid-refactor.

---

## What we have today

Geometry, GPU resources, transform, material, textures, *and* draw logic all live in
one class hierarchy: `Drawable` → `Object` / `Cube` / `Rect` / `Tri`.

The tell: `cube.cpp` and `object.cpp` have **nearly identical `init()` and `draw()`
bodies** — same VAO/VBO/EBO generation, same three `glVertexAttribPointer` calls, same
shader-selection + uniform + texture-bind + `glDrawElements` sequence. Every new
primitive copy-pastes ~70 lines. That duplication is what the roadmap's "real `Mesh`
abstraction" item is pointing at, and it is the likely root of the `TODO.md`
"mishandling of VBO" bug.

### Problems baked into the current design

1. **One class does five jobs.** `Drawable` holds geometry (`vertices`/`indices`), GPU
   handles (`VAO`/`VBO`, plus `EBO` bolted onto subclasses separately), a transform
   (`pos`/`rotation`/`scale`), a material, textures, *and* a shader — then `draw()` also
   sets camera/light uniforms. Geometry has no business knowing about point lights.
2. **No RAII / ownership story.** Buffers are created with `glGenBuffers` but never
   `glDeleteBuffers`'d. If a `Drawable` is ever copied (and they are global values in
   `main.cpp`), two objects share the same `VAO`/`VBO` handles → double-bind,
   double-free territory. A classic source of "VBO mishandling."
3. **Magic numbers & drift.** `Cube::draw` hardcodes `glDrawElements(..., 36, ...)`,
   `Rect` hardcodes `6`; only `Object` uses `indices.size()`. The cube's attribute-1
   comment says `// vertex col` but it is actually the normal — the layout has already
   drifted from the docs.
4. **Geometry kept in RAM forever**, even after upload, with no reason to.
5. **Per-object redundant uniform uploads.** view/projection/lights get re-sent for
   every object every frame, instead of once per shader.

---

## What a proper `Mesh` class looks like

Core idea: **separation of concerns**. A `Mesh` owns *only* GPU geometry and knows how
to issue one draw call. Everything else — transform, material, lighting — lives
elsewhere and is *composed*.

```cpp
// Mesh.h — owns GPU geometry ONLY. No transform, no material, no lights.
class Mesh {
public:
    Mesh(std::vector<VertexAttribute> verts, std::vector<u32> indices);
    ~Mesh();                                   // glDeleteBuffers / glDeleteVertexArrays

    Mesh(const Mesh&)            = delete;      // GL handles aren't copyable...
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) noexcept;                      // ...but they're movable (RAII)
    Mesh& operator=(Mesh&&) noexcept;

    void draw() const;                          // bind VAO + glDrawElements(indexCount)

    // factory helpers replace the Cube/Rect/Tri subclasses:
    static Mesh cube();
    static Mesh fromOBJ(const std::filesystem::path& path, bool genNormals = false);

private:
    u32 vao = 0, vbo = 0, ebo = 0;
    u32 indexCount = 0;
    void upload(const std::vector<VertexAttribute>&, const std::vector<u32>&);
};
```

```cpp
void Mesh::draw() const {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
```

The attribute layout (`init()`) gets written **once**, in `Mesh::upload`, instead of
being copy-pasted into every primitive.

The thing `Cube`/`Object` currently *are* — a positioned, materialed instance —
becomes a small composition:

```cpp
struct Transform { Vec3 pos, rotation, scale{1,1,1}; Mat modelMat() const; };

struct RenderObject {           // (a.k.a. Model / Entity)
    Mesh*     mesh;             // shared, non-owning — many objects, one cube mesh
    Material  material;
    Transform transform;
    std::vector<Texture*> textures;
};
```

And the renderer drives it — setting *shared* uniforms once, per-object uniforms per
object:

```cpp
void Window::draw(const RenderObject& obj) {
    shader.use();
    shader.setMat4(SHADER_MODEL_SET_UNIFORM, obj.transform.modelMat());
    shader.setMaterial(obj.material);
    for (size_t i = 0; i < obj.textures.size(); ++i) obj.textures[i]->bind(i);
    obj.mesh->draw();
}
// view / projection / lights / camera pos set ONCE per frame, before the loop.
```

---

## The differences, summarized

| | Current `Drawable`/`Object`/`Cube` | Proper `Mesh` + composition |
|---|---|---|
| Responsibilities | Geometry + GPU + transform + material + textures + lighting, in one class | `Mesh` = GPU geometry only; transform/material/lighting separated |
| Primitives | `Cube`/`Rect`/`Tri` subclasses, each re-implementing `init`/`draw` | Static factory functions (`Mesh::cube()`); zero subclassing |
| Buffer setup code | Copy-pasted ~70 lines per type | Written once in `Mesh::upload` |
| Ownership | Raw handles, never freed, unsafe to copy | RAII: freed in destructor, move-only |
| **Mesh reuse** | One cube mesh per `Cube` object — 100 cubes = 100 VBOs | One `Mesh`, shared by N `RenderObject`s (the path to instancing) |
| Uniforms | view/proj/lights re-sent per object | shared uniforms once/frame, per-object uniforms per draw |
| Index count | Hardcoded `36`/`6` | Stored `indexCount` |

The biggest conceptual win — and why the roadmap wants this **before** PBR/shadows/
deferred — is the **shared, non-owning mesh**. Once geometry is decoupled from instance
state, "draw this one mesh 10,000 times with different transforms" (GPU instancing,
Milestone 5) and "build a G-buffer pass that iterates all meshes" (deferred, Milestone
3) become natural. With the current design, every instance is welded to its own
buffers, so those features have nowhere to hook in.

---

## Migration plan

A reasonable first cut, ordered so the build keeps working at every step:

1. **Add `Mesh`** with RAII (move-only, `glDeleteBuffers` in the destructor) and the
   `cube()` / `fromOBJ()` factories. Write the vertex-attribute layout once in
   `upload()`.
2. **Introduce `Transform`** (extract `pos`/`rotation`/`scale` + `getModelMat()` out of
   `Drawable`) and **`RenderObject`** (mesh + material + transform + textures).
3. **Move shared uniforms** (view, projection, lights, camera position) into a
   once-per-frame setup in `Window`, out of the per-object `draw()` path.
4. **Add `Window::draw(const RenderObject&)`** that sets only per-object uniforms and
   calls `mesh->draw()`.
5. **Migrate `main.cpp`** to construct `Mesh`es + `RenderObject`s instead of global
   `Cube`/`Object` instances.
6. **Delete `Drawable`/`Object`/`Cube`/`Rect`/`Tri`** once nothing references them.

Keep `Drawable` and the primitives around until step 5 is done so nothing breaks
mid-refactor.

### Follow-on cleanups this unlocks
- Drop CPU-side `vertices`/`indices` after upload (or keep behind a flag for meshes that
  need CPU access).
- Replace the per-vertex O(V·F) `generateNormals()` with a single O(F) pass that
  accumulates face normals into a per-vertex array.
- GPU instancing for repeated meshes (Milestone 5).
