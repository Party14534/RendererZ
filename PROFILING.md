# Profiling

## Build types

CMake build type controls both optimization level and whether debug symbols
are kept. Pick the one that matches what you're doing:

| Build type        | Flags                              | Use for                        |
|--------------------|-------------------------------------|---------------------------------|
| `Release`          | `-O3 -DNDEBUG`                       | Normal use, benchmarking wall time |
| `RelWithDebInfo`    | `-O3 -g -fno-omit-frame-pointer`     | Profiling (VTune, perf)         |
| `Debug`             | `-O0 -g`                             | Debugging with gdb/lldb         |

`RelWithDebInfo` is overridden in `CMakeLists.txt` to match `Release`'s `-O3`
(CMake's stock default is `-O2`) so you're profiling the same codegen that
ships, just with symbols kept for source/line attribution. It also keeps
frame pointers, which makes stack unwinding for hardware-event sampling
(VTune hotspots, `perf record -g`) reliable.

```bash
# Normal build
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build -j

# Profiling build
cmake -B build-profile -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build-profile -j
```

Use a separate build directory (`build-profile`) rather than reconfiguring
`build/` in place, so you don't have to keep flipping build types back and
forth between normal runs and profiling sessions.

## Intel VTune (CPU)

Install oneAPI VTune Profiler, then source its environment once per shell:

```bash
source /opt/intel/oneapi/vtune/latest/vtune-vars.sh
```

Collect against the profiling build. The renderer runs an interactive GLFW
loop rather than exiting on its own, so just close the window once you've
captured enough frames — VTune records until the process exits.

```bash
# Where is time going, generally
vtune -collect hotspots -result-dir ./vtune_hotspots -- ./renderer

# TBB/OpenMP thread behavior - stalls, imbalance, actual TBB task attribution
vtune -collect threading -result-dir ./vtune_threading -- ./renderer

# Cache misses / memory-bound analysis - often more revealing than hotspots
# for math-heavy CPU code (matrix/vertex work) feeding the GPU
vtune -collect memory-access -result-dir ./vtune_memaccess -- ./renderer
```

View results:

```bash
vtune-gui ./vtune_hotspots
# or, text summary:
vtune -report hotspots -r ./vtune_hotspots
```

## Intel VTune GPU analysis - Intel GPUs only

VTune's GPU analysis types (`gpu-offload`, `gpu-hotspots`) rely on Intel's
Metrics Discovery library and driver-level instrumentation exposed through
the Intel graphics driver (via Level Zero / OpenCL / media APIs). That
instrumentation doesn't exist in NVIDIA's driver stack, so **VTune cannot
show NVIDIA GPU-side metrics** (kernel occupancy, SM utilization, GPU
timeline, etc.) - only Intel integrated/discrete GPUs support those
collection types.

Against an NVIDIA GPU, VTune still works for everything on the CPU side of
this project (draw call submission overhead, matrix/vertex prep, driver API
call costs as seen from the CPU) via `hotspots`/`threading`/`memory-access`
above - it just can't see inside the GPU itself.

For actual GPU-side profiling (frame timeline, shader/kernel occupancy, API
call breakdown) on an NVIDIA GPU, use NVIDIA's own tools instead:

- **Nsight Systems** - system-wide timeline (CPU + GPU + driver), good first
  stop for "what's actually taking the time":
  ```bash
  nsys profile -o renderer_trace ./build-profile/renderer
  nsys-ui renderer_trace.nsys-rep
  ```
- **Nsight Graphics** - frame-by-frame GPU capture for an OpenGL app like
  this one (draw call inspection, shader profiling, pipeline state). Launch
  it and use "Activity: Frame Debugger/Profiler" against `renderer`.
- **NVIDIA Nsight Compute** - deep per-kernel analysis; more relevant to CUDA
  compute kernels than an OpenGL rasterization pipeline like this one, but
  available if that changes.

Both Nsight Systems and VTune can be run against the same binary in
back-to-back sessions - use VTune for the CPU-side story and Nsight Systems
for the GPU-side story, then correlate by wall-clock timestamps.
