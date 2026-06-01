# RendererZ — Resume Roadmap

The target: turn the "(in progress)" entry into the strongest project on the resume.
Each milestone below ends with the **exact bullet you've earned the right to write**.
Build top to bottom — later milestones depend on earlier ones.

Rule: every milestone ships a **screenshot or short GIF** into the README. Graphics
is a visual field; bullets without pixels are half-credit.

---

## Milestone 0 — Get to 3D (prerequisite plumbing, NOT a resume bullet)

This is the gap between what you have (2D textured quads, CPU-side transforms) and
anything a graphics team cares about. No bullet here — it's the price of entry.

- [ ] Perspective camera: view + projection matrices, a free-fly/orbit camera with input.
- [ ] Move transforms to the GPU — upload model/view/projection as uniforms instead of
      multiplying vertices on the CPU. (Your CPU `Mat` library stays for camera/scene math.)
- [ ] Enable the depth buffer (`glEnable(GL_DEPTH_TEST)`).
- [ ] Load real meshes: an OBJ loader first, then glTF 2.0 (glTF gets you PBR materials,
      normals, and tangents for free later — worth doing).
- [ ] Index buffers (EBO) and a real `Mesh` abstraction.

When this works (a textured 3D model spinning with a real camera), you can already
upgrade the current bullets to past tense and add:
> Render indexed 3D meshes loaded from glTF/OBJ with a perspective camera and
> GPU-side MVP transforms.

---

## Milestone 1 — Lighting & PBR  ← the single most important one

This is table stakes. Without PBR a "renderer" reads as a tutorial; with it, it reads
as a graphics engineer's project.

- [ ] Blinn-Phong first (fast win, proves the lighting loop works).
- [ ] Normal mapping in tangent space (compute/load tangents, TBN matrix in the shader).
- [ ] Cook-Torrance PBR: metallic/roughness workflow, GGX distribution, Smith geometry,
      Fresnel-Schlick.
- [ ] Image-Based Lighting: irradiance map (diffuse) + prefiltered env map & BRDF LUT
      (specular) from an HDR equirectangular environment.

**Earned bullets:**
> Implemented a physically-based shading pipeline (Cook-Torrance, GGX, metallic-roughness)
> with tangent-space normal mapping and image-based lighting (irradiance + prefiltered
> specular + BRDF LUT) from HDR environment maps.

---

## Milestone 2 — Shadows

- [ ] Basic shadow mapping for a directional light (render depth from light, sample in
      the main pass).
- [ ] Fix the obvious artifacts: depth bias for acne, PCF for soft edges.
- [ ] Cascaded Shadow Maps (CSM) — split the frustum, one shadow map per cascade.
      CSM is a name interviewers recognize instantly.

**Earned bullet:**
> Added real-time shadows via cascaded shadow maps with PCF filtering and depth-bias
> tuning to eliminate acne and peter-panning artifacts.

---

## Milestone 3 — Rendering architecture

Shows you understand the engine *as a system*, not just shaders.

- [ ] Deferred renderer: a G-buffer pass (albedo, normal, position/depth, material) then
      a lighting pass. (Or Forward+ if you'd rather — pick one and know the tradeoff.)
- [ ] Support many lights efficiently — the whole point of going deferred.

**Earned bullet:**
> Architected a deferred rendering pipeline (G-buffer + screen-space lighting pass)
> supporting hundreds of dynamic lights; documented the forward-vs-deferred tradeoff.

---

## Milestone 4 — HDR + post-processing

- [ ] Render to a floating-point HDR framebuffer.
- [ ] Tonemapping (ACES or Reinhard) + gamma correction.
- [ ] Bloom (bright-pass + Gaussian/dual-filter downsample-upsample).
- [ ] Anti-aliasing: FXAA (easy) or TAA (impressive, harder).

**Earned bullet:**
> Built an HDR post-processing chain — ACES tonemapping, physically-based bloom, and
> FXAA/TAA anti-aliasing — rendering to floating-point framebuffers.

---

## Milestone 5 — Pick ONE depth/differentiator (don't do all)

Choose the one that excites you; depth in one beats shallow in all.

- [ ] **Compute shaders**: GPU particle system or GPU-driven culling. Proves you see the
      GPU as a parallel machine, not just a rasterizer. → *"GPU particle system / culling
      in compute shaders."*
- [ ] **SSAO + SSR**: screen-space ambient occlusion and reflections. → *"Screen-space
      ambient occlusion and reflections."*
- [ ] **Frustum + occlusion culling** with instanced rendering for large scenes. →
      *"Frustum culling and GPU instancing scaling to N draw calls / M instances."*
- [ ] **A SIGGRAPH/known-technique implementation** with a write-up (e.g. voxel-cone GI,
      light propagation volumes). The write-up proves paper-reading ability.

---

## Milestone 6 — Modern API (optional, strong differentiator)

OpenGL is fine for fundamentals, but Vulkan/DX12 is increasingly expected.

- [ ] Port a slice of the renderer to Vulkan, OR build a small standalone Vulkan renderer.
      Even a textured-model-with-one-light Vulkan app is a real talking point.

**Earned bullet:**
> Built a Vulkan renderer (explicit synchronization, descriptor sets, render passes)
> to gain modern explicit-API experience.

---

## The "done" RendererZ resume entry (target state)

When Milestones 0–4 + one of 5 are done, the entry becomes:

> **RendererZ — Real-Time PBR Renderer (C++ / OpenGL)**
> - Built a from-scratch real-time renderer: glTF mesh loading, perspective camera,
>   and GPU-side transforms.
> - Implemented physically-based shading (Cook-Torrance, GGX, metallic-roughness) with
>   tangent-space normal mapping and image-based lighting from HDR environments.
> - Added cascaded shadow maps with PCF, and a deferred lighting pipeline supporting
>   hundreds of dynamic lights.
> - Built an HDR post chain: ACES tonemapping, bloom, and FXAA.
> - [your Milestone 5 choice].

---

## Order of operations / focus

1. **Milestone 0** — nothing else matters until you're in 3D. Do this first, this month.
2. **Milestone 1 (PBR)** — the highest-value single thing on this list.
3. **Milestones 2 → 3 → 4** in order.
4. **One** of Milestone 5.
5. Milestone 6 only if you want the modern-API edge.

Write a short blog post per milestone as you finish it. For a self-taught graphics
candidate, the write-ups are worth nearly as much as the code — they prove you can read
a technique and explain it.
