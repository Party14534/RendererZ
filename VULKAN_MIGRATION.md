# RendererZ — OpenGL → Vulkan Migration Plan

This document assesses what it would take to port RendererZ from OpenGL 3.3 Core to
Vulkan, based on the current state of the codebase (commit `2b493bb`, "Added point
cloud support"). It's meant to be read alongside [`RESUME_ROADMAP.md`](RESUME_ROADMAP.md),
which already lists a Vulkan port as **Milestone 6** — this doc goes one level deeper
into *how*, not just *whether*.

---

## 1. Current State Summary

RendererZ is a small (~2,900 LOC in `src/`), single-executable C++17 renderer:

- **Windowing**: GLFW, system-installed (found via `find_package`/`pkg-config`), no
  vendoring.
- **Context**: OpenGL 3.3 Core Profile, forward-compatible on macOS
  (`src/global.cpp::initializeGL()`).
- **Loader**: GLAD, vendored as flat source in `include/` (`glad.c`/`glad.h`), generated
  for the (mismatched but harmless) compatibility-profile API surface.
- **Build**: single CMake target, no package manager, no submodules. macOS-specific
  branches for compiler selection, framework linking (`Cocoa`/`IOKit`/`CoreVideo`/`OpenGL`),
  and OpenMP/parallel-STL support.
- **Rendering**: forward, single-pass, straight to the default framebuffer. No FBOs, no
  MSAA, no compute shaders, no instancing, no post-processing, no ImGui.
- **Shading**: Blinn-Phong (1 directional + up to 4 point lights), skybox
  reflection/refraction, a fixed vertex layout (pos/normal/uv, 8 floats) for all solid
  geometry, and a separate point-cloud path (pos/color, 6 floats, `GL_POINTS`).
- **Assets**: custom (non-Assimp) `.obj` parser, no glTF.

GL calls are reasonably well-contained to five places: `Window`, `Shader`, `Mesh`/
`PointMesh`, and `Texture`/`CubeMap`. Everything else (`Drawable`, `Camera`, the custom
`Math` library, `ObjectLoading`, `Input`) is GL-agnostic. **There is no existing
render-abstraction layer** — no command buffers, no descriptor/pipeline objects, no
explicit synchronization, no notion of a render pass distinct from "the one draw loop."
This is the single most important fact for scoping the migration: it is not a matter of
swapping a backend behind an existing interface, it's building the interface for the
first time, in Vulkan's vocabulary.

Two pieces of existing tech debt are directly relevant and worth fixing *during* the
port rather than carrying forward:

1. **Shader source has two unsynchronized copies.** The `.vert`/`.frag` files in
   `src/Shaders/` are the "readable" source, but the code path actually used at runtime
   loads hand-copied raw-string duplicates from `src/Shaders/ShaderCode/*.cpp`
   (`extern std::string objectVertex`, etc.). Nothing keeps these in sync.
2. **`Texture`'s sampler-parameter setters are stubs.** `setTextureParameter`/
   `setTextureFilter`/`setMipMapFilter` in `src/Objects/Texture.h` store a value but
   never call any `glTexParameteri` — wrap/filter mode is effectively hardcoded today.

---

## 2. Conceptual Mapping: OpenGL → Vulkan

This is the shape of the rewrite. Nothing here is optional — Vulkan has no
implicit/global-state equivalent for any of these, which is *the* reason a Vulkan port
of a small renderer takes disproportionately more code than the LOC count suggests.

| OpenGL concept (implicit / global) | Vulkan concept (explicit, must be built) |
|---|---|
| Bound context, implicit device | `VkInstance` → `VkPhysicalDevice` → `VkDevice` + queues (graphics/present, possibly separate transfer) |
| `glfwMakeContextCurrent` + swap | `VkSurfaceKHR` (via `glfwCreateWindowSurface`) + `VkSwapchainKHR`, manual image acquire/present |
| Default framebuffer | Swapchain images + `VkImageView`s + `VkFramebuffer`s bound to a `VkRenderPass` (or dynamic rendering, see §5) |
| `glClear`/`glViewport`/immediate draw calls | `VkCommandBuffer` recorded per frame: begin render pass → bind pipeline → bind descriptor sets → bind vertex/index buffers → `vkCmdDrawIndexed` → end |
| Shader compile at runtime (`glCompileShader`, GLSL text) | Offline compile to SPIR-V (`glslc`/`glslangValidator`) at build time, loaded as `VkShaderModule` bytecode |
| `glUseProgram` + ad hoc `glUniform*` per draw | `VkPipeline` (baked fixed-function + shader-stage state) + `VkPipelineLayout` + `VkDescriptorSet` (UBOs/samplers bound explicitly, updated per-frame via descriptor writes or push constants) |
| VAO/VBO/EBO (`glGenBuffers`, `glBufferData`) | `VkBuffer` + `VkDeviceMemory` (or VMA allocation), explicit staging-buffer upload for `DEVICE_LOCAL` memory, manual vertex-input-state description in the pipeline |
| `glTexImage2D`, `glGenerateMipmap`, sampler state | `VkImage` + `VkImageView` + `VkSampler`, explicit layout transitions (`UNDEFINED` → `TRANSFER_DST` → `SHADER_READ_ONLY`) via pipeline barriers, manual mip generation (blit loop) |
| GPU/driver ordering guaranteed by GL | Explicit synchronization: `VkFence` (CPU↔GPU), `VkSemaphore` (GPU↔GPU, image-acquired/render-finished), pipeline barriers for every resource-state transition |
| `glGetError` / driver validation | Vulkan Validation Layers (`VK_LAYER_KHRONOS_validation`) — required in debug builds since the API itself does almost no runtime checking |
| Global `glEnable(GL_DEPTH_TEST)`/`glDepthFunc` toggles | Baked into `VkPipeline` creation (`VkPipelineDepthStencilStateCreateInfo`) — changing "depth func for the skybox" now means a second pipeline object, not a state toggle |
| N/A (loader = GLAD) | Vulkan loader: **volk** (recommended) or the Vulkan SDK's static loader, plus **MoltenVK** on macOS |
| N/A | GPU memory allocator — hand-rolling `vkAllocateMemory` per-resource is a well-known anti-pattern (driver limits, ~4096 max allocations); use **VMA** (Vulkan Memory Allocator, header-only, AMD) |

---

## 3. File-by-File Impact

| File | Disposition |
|---|---|
| `src/System/Window.h` / `window.cpp` | **Full rewrite.** Becomes the home of instance/device/swapchain/render-pass creation, per-frame acquire/submit/present, and all synchronization objects. This is the largest single piece of new code in the port. |
| `src/Shaders/Shaders.h` / `shaders.cpp` | **Full rewrite.** Becomes pipeline + descriptor-set-layout + descriptor-set management. Uniform upload changes from "call `glUniform*` with a string name" to "write a UBO struct + `vkUpdateDescriptorSets`" or push constants for small/hot data (model matrix is a good push-constant candidate). |
| `src/Shaders/*.vert` / `*.frag` | **Kept, but change compilation model.** GLSL source stays close to as-is (Vulkan GLSL needs minor changes: explicit `layout(location=n)`/`layout(binding=n)` qualifiers, no implicit uniform locations). Compiled to SPIR-V at build time instead of at runtime. |
| `src/Shaders/ShaderCode/*.cpp` (raw-string duplicates) | **Deleted.** Their entire reason to exist (avoiding a file-read at runtime) goes away once shaders are precompiled SPIR-V loaded as binary blobs — this removes the two-sources-of-truth problem for free. |
| `src/Objects/Mesh.h` / `mesh.cpp` | **Rewrite internals, keep the public shape.** VAO concept disappears entirely (Vulkan has no VAO equivalent — vertex layout lives in the pipeline). VBO/EBO become `VkBuffer`s with explicit staging uploads. `PointMesh`'s per-frame `glBufferSubData` becomes either a persistently-mapped `HOST_VISIBLE|HOST_COHERENT` buffer or a staging-buffer copy each frame. |
| `src/Objects/Texture.h` / `texture.cpp` | **Rewrite internals.** `glTexImage2D` → staging buffer + `VkImage` + layout-transition barriers + `vkCmdCopyBufferToImage`. Mip generation becomes an explicit blit loop. This is also the natural point to *actually implement* `setTextureParameter`/`setTextureFilter` as real `VkSamplerCreateInfo` fields instead of carrying forward the current no-op stubs. |
| `src/Objects/skyBox.cpp` / `SkyBox.h` | **Moderate rewrite.** Cubemap becomes a `VkImage` with `VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT` and 6 array layers instead of 6 separate `glTexImage2D` calls. |
| `src/Objects/drawable.cpp` / `Drawable.h` | **Small changes.** Still composes mesh + material + textures + transform; draw dispatch changes from "bind shader, set uniforms, `mesh->draw()`" to "bind pipeline, bind descriptor set, bind vertex/index buffers, `vkCmdDrawIndexed`" — same shape, different verbs. |
| `src/Objects/lightSource.cpp` / `LightSource.h` | **No change.** Pure data holder, no GL dependency. |
| `src/Objects/primitives.cpp` / `Primitives.h` | **No change.** Hardcoded vertex/index arrays are API-agnostic. |
| `src/Tools/objectLoading.cpp` / `ObjectLoading.h` | **No change.** Produces plain vertex/index arrays; API-agnostic. |
| `src/System/Camera.h` / `camera.cpp` | **No change.** One caveat: Vulkan's clip-space Z range is `[0,1]` vs. GL's `[-1,1]`, and Vulkan's clip-space Y is flipped relative to GL — the projection matrix needs a one-time fix (either flip Y in the matrix or in the viewport, and adjust the perspective-matrix Z term). This is a well-known gotcha, not a redesign. |
| `src/Math/*` | **No change.** Pure linear algebra, API-agnostic (aside from the clip-space note above, applied at the call site that builds the projection matrix). |
| `src/Input/*`, `src/System/Events.h` | **No change.** |
| `src/main.cpp` | **Small changes.** Scene-setup code stays the same shape; anything touching `win.clear()/draw()/display()` adapts to whatever the new `Window`/`Renderer` per-frame API looks like. |
| `include/glad.c` / `glad.h` | **Deleted**, replaced by a Vulkan loader (volk recommended: header-only-ish, no codegen step, dynamically loads instance/device function pointers — avoids linking directly against `libvulkan`). |
| `CMakeLists.txt` | **Moderate changes.** Add Vulkan SDK dependency (`find_package(Vulkan REQUIRED)`), add a SPIR-V compile step (custom command running `glslc` per shader file, or a CMake shader-compile helper), vendor **VMA** (single header, easy) and **volk** (small, easy), keep the macOS framework-linking branch but drop the `-framework OpenGL` line, and on macOS specifically ensure `MoltenVK` is discoverable (via the Vulkan SDK's macOS install, which bundles it) and that `VK_KHR_portability_subset`/`VK_KHR_portability_enumeration` are enabled since MoltenVK is a non-conformant, subset implementation. |

**Net effect**: 4 files/pairs need a full rewrite (`Window`, `Shaders`, `Mesh`, `Texture`),
2 need moderate rewrites (`SkyBox`, `CMakeLists.txt`), 1 needs small adaptation
(`Drawable`), and the rest (`Camera`, `Math`, `ObjectLoading`, `Input`, `LightSource`,
`Primitives`, `main.cpp`'s scene-setup logic) are largely untouched. The *volume* of
code affected is maybe a third of the codebase, but the *new concepts* required
(descriptor sets, pipelines, explicit sync, memory allocation) don't exist anywhere
today, so this reads as "build a Vulkan backend from zero, then re-plumb the existing
call sites into it" rather than "port existing logic 1:1."

---

## 4. New Dependencies

| Dependency | Purpose | Integration effort |
|---|---|---|
| **Vulkan SDK** (LunarG) | Headers, validation layers, `glslc`/`glslangValidator`, and on macOS, bundled **MoltenVK** | Install once (`brew install vulkan-tools`... actually the SDK is a separate LunarG installer or `brew install --cask vulkan-sdk` equivalent); CMake needs `find_package(Vulkan REQUIRED)` |
| **volk** (or link `libvulkan` directly) | Loads Vulkan function pointers per-instance/per-device without a hard link dependency | Single header + single source file, drop into `include/` next to where `glad.c`/`glad.h` currently live |
| **VMA** (Vulkan Memory Allocator) | Sub-allocates `VkDeviceMemory`, avoids the ~4096-allocation driver limit, handles staging/upload patterns | Single header (`vk_mem_alloc.h`), one `.cpp` with `VMA_IMPLEMENTATION` defined |
| **glslc** (ships with the SDK) | Compiles GLSL → SPIR-V at build time | CMake custom command/target per shader; output `.spv` files embedded as resources or loaded from disk at startup |
| GLFW (already present) | No swap — same library, just uses `GLFW_INCLUDE_VULKAN` + `glfwCreateWindowSurface` instead of the GL context path. `find_package(glfw3)` stays the same. | None — this is the pleasant surprise of the migration |
| (Optional) **Dear ImGui** | Not present today, but if debug UI is wanted, `imgui_impl_vulkan` is more setup (needs its own descriptor pool/render pass wiring) than `imgui_impl_opengl3` | Only relevant if this is added alongside the port |

No package manager exists in this project today (deps are either system-installed via
`find_package`/`pkg-config`, or vendored flat source in `include/`). The Vulkan port
doesn't require changing that policy — volk and VMA are both small enough to vendor the
same way GLAD/stb_image are today. The Vulkan SDK itself (for headers/validation
layers/glslc) would be the one new *system-level* install, analogous to how GLFW is
already expected to be brew-installed.

---

## 5. Design Decisions to Make Up Front

These are choices with no "obviously correct" answer for a project this size — worth
deciding deliberately rather than defaulting into:

1. **Dynamic Rendering vs. classic `VkRenderPass`/`VkFramebuffer`.**
   `VK_KHR_dynamic_rendering` (core in Vulkan 1.3) skips render-pass/framebuffer objects
   entirely in favor of specifying attachments directly at `vkCmdBeginRendering` time.
   For a single-pass forward renderer with no G-buffer, this is meaningfully less
   boilerplate and is the modern recommended default for new code — **recommended**,
   provided the target Vulkan version (via MoltenVK) supports it. MoltenVK has supported
   `VK_KHR_dynamic_rendering` since a recent version; verify against the installed SDK.
2. **Minimum Vulkan version / MoltenVK version pin.** Since macOS is the primary dev
   platform, functionality is capped by whatever MoltenVK exposes on top of Metal —
   check MoltenVK's supported-extensions list before assuming any given Vulkan 1.2/1.3
   feature (e.g. dynamic rendering, descriptor indexing) is available.
3. **Descriptor strategy.** With ~17 drawable objects and 4 shader programs, a simple
   approach (one descriptor set per material/texture-combo, updated once at load time,
   plus a per-frame UBO for camera/light data bound via a separate set) is enough —
   no need for bindless/descriptor-indexing at this scale.
4. **Push constants for per-draw data.** The model matrix + material scalars change
   every draw call; pushing them as push constants (guaranteed ≥128 bytes on all
   implementations) avoids descriptor-set updates in the hot path and maps naturally
   onto the current `Shaders::setMat4(SHADER_MODEL_SET_UNIFORM, ...)` per-draw pattern.
5. **Buffer allocator: VMA, not hand-rolled.** Given `Mesh`'s existing RAII pattern
   (move-only, dtor frees GL handles), VMA's `VmaAllocation` handle slots into the same
   shape — same RAII idea, different handle type.
6. **What happens to the point-cloud CPU physics.** `applyGravity()`'s O(n²) CPU
   simulation (already parallelized via `std::execution::par`/TBB) is a natural compute-
   shader candidate once Vulkan compute is available — worth flagging as a *follow-on*
   task, not part of the initial port (the initial port should just get point rendering
   working via a per-frame `HOST_VISIBLE` buffer update, matching today's
   `glBufferSubData` behavior, before optimizing).

---

## 6. Suggested Milestones

Sized against the existing `RESUME_ROADMAP.md` milestone style. Each is independently
demoable/screenshot-able.

### V0 — Vulkan "Hello Triangle" inside RendererZ's window
Instance/device/validation-layers/swapchain/surface creation via the existing GLFW
window, one hardcoded triangle (no vertex buffer yet — hardcoded in the vertex shader),
one render pass or dynamic-rendering pass, present loop with correct fence/semaphore
sync. Proves the whole device/swapchain/sync skeleton works before any engine code is
touched.

### V1 — Real vertex/index buffers + one textured mesh
Staging-buffer upload path (`VkBuffer` + VMA), one `Mesh` rendered with the current
pos/normal/uv layout, one `Texture` loaded through a staging image + layout transitions,
one descriptor set for the sampler, uniform buffer for MVP matrices. This is the point
where `Mesh`/`Texture`/`Shaders` get their real rewrites.

### V2 — Full scene parity with current OpenGL renderer
All ~17 drawables, the skybox (cubemap + depth-func-equivalent pipeline), Blinn-Phong
lighting (directional + point lights via UBO), and the point cloud
(`PointMesh`/`GL_POINTS` → `VK_PRIMITIVE_TOPOLOGY_POINT_LIST`, per-frame buffer update).
At this point RendererZ has visual parity with the current OpenGL build, on Vulkan.

### V3 — Cleanup / cutover
Delete the OpenGL path (`glad.c`/`glad.h`, GL-specific code in `Window`/`Shaders`/`Mesh`/
`Texture`), delete the `ShaderCode/*.cpp` raw-string duplicates now that SPIR-V is
build-time-compiled from the single `.vert`/`.frag` source of truth, implement the
`Texture` sampler-parameter setters for real (they're currently no-ops), update
`RESUME_ROADMAP.md` to mark Milestone 6 done.

### (Optional, follow-on) V4 — Vulkan-specific differentiator
Move the point-cloud gravity simulation into a compute shader (this doubles as
Milestone 5's "GPU particle system / culling in compute shaders" option) — now that
compute is available, this is a natural place to spend the "pick one differentiator"
budget from Milestone 5, rather than treating Vulkan and Milestone 5 as unrelated line
items.

---

## 7. Effort & Risk Notes

- **Codebase size is favorable.** ~2,900 LOC and a GL surface concentrated in 4-5
  files means there's very little to *untangle* — the risk isn't "GL calls are
  scattered everywhere," it's "several core concepts (sync, descriptor sets, memory
  management, pipeline objects) don't exist yet and each has real failure modes if
  done wrong" (validation-layer errors, hangs from missing barriers, GPU crashes from
  incorrect memory type usage).
- **macOS-via-MoltenVK is an added variable.** MoltenVK is a translation layer, not a
  native Vulkan driver — some extensions/features lag or behave subtly differently than
  on Windows/Linux with a native driver. Budget time for occasional
  works-on-Linux-doesn't-work-via-MoltenVK debugging, and lean on the validation layers
  aggressively (`VK_LAYER_KHRONOS_validation` catches most portability issues before
  they become silent Metal-side failures).
- **Debugging is harder without ImGui/RenderDoc-equivalent.** RenderDoc does support
  Vulkan-on-Metal-via-MoltenVK capture, but it's worth confirming this works in your
  setup early (V0 stage) rather than discovering a tooling gap mid-port. Xcode's Metal
  debugger can also inspect MoltenVK's translated Metal commands as a fallback.
- **This is a rewrite of the rendering backend, not a refactor.** Expect to keep the
  OpenGL path buildable (e.g. behind a CMake option or a separate branch) until V2
  reaches parity, so there's always a working reference build to compare against for
  correctness (lighting math, texture appearance, etc. shouldn't change — if the Vulkan
  image looks different, that's a bug, not a stylistic difference).
- **Strategic timing question, raised by the existing roadmap itself.** `RESUME_ROADMAP.md`
  orders Vulkan last (Milestone 6), after PBR, shadows, deferred rendering, and
  post-processing — deliberately, since those are higher resume-value and this project
  is explicitly framed as resume-building. Porting to Vulkan *now*, while the codebase
  is small (~2,900 LOC, one lighting model, no deferred/shadow/post passes yet) is
  architecturally cheaper than porting *later* once G-buffers, shadow maps, and a bloom
  chain all exist on top of the current OpenGL abstractions — but it also means
  redoing more OpenGL-side feature work in Vulkan afterward instead of once. There's no
  wrong answer here, but it's worth deciding deliberately rather than by default:
  smaller-now-but-more-total-rewrite vs. bigger-later-but-do-each-feature-once.

---

## 8. Recommended Reading / Reference

- Vulkan Tutorial (vulkan-tutorial.com) — the standard "hello triangle to textured mesh"
  walkthrough; matches the V0/V1 milestones above closely.
- `vkguide.dev` — more opinionated, modern (dynamic rendering, VMA-first) approach; a
  good complement since it assumes the render-pass-free style recommended in §5.
- MoltenVK's own `docs/MoltenVK_Runtime_UserGuide.md` (ships with the SDK) — the
  authoritative list of which extensions/features are supported on macOS, worth
  checking before committing to any Vulkan 1.3-only feature.
