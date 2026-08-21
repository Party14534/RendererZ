cmake_minimum_required(VERSION 3.16)

# To build: cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && cmake --build build -j
# For profiling builds (VTune, perf, etc.) see PROFILING.md.

# --- Compiler selection -------------------------------------------------------
# This must happen BEFORE project(), since CMake locks in the compiler there.
# On macOS we use clang (Apple's toolchain / the GL+GLFW path expects it).
# Everywhere else we prefer g++ when it's available, otherwise fall back to
# whatever CMake detects. An explicit CXX env var or -DCMAKE_CXX_COMPILER wins.
if(NOT DEFINED CMAKE_CXX_COMPILER AND NOT DEFINED ENV{CXX})
    if(APPLE)
        find_program(_RZ_CLANGXX clang++)
        if(_RZ_CLANGXX)
            set(CMAKE_CXX_COMPILER "${_RZ_CLANGXX}")
        endif()
    else()
        find_program(_RZ_GXX g++)
        if(_RZ_GXX)
            set(CMAKE_CXX_COMPILER "${_RZ_GXX}")
        endif()
    endif()
endif()

project(RendererZ LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# --- Sources ------------------------------------------------------------------
# Discover the project sources automatically. CONFIGURE_DEPENDS makes CMake
# re-run the glob when files are added/removed, so the file list never has to
# be maintained by hand. glad.c lives under include/ and is added explicitly.
file(GLOB_RECURSE RENDERER_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# src/temp/ holds experimental scratch code that must not be compiled into the
# renderer (it duplicates symbols from the real sources). Filter it out.
list(FILTER RENDERER_SOURCES EXCLUDE REGEX "${CMAKE_SOURCE_DIR}/src/temp/")

# --- Shader embedding ----------------------------------------------------------
# Shaders live as plain .frag/.vert files, but the renderer needs them baked
# into the binary as strings (so consumers of this project don't have to carry
# loose shader files around). Instead of hand-copying each one into a
# std::string literal, generate that .cpp from the real shader file at build
# time - the .frag/.vert file is the only copy anyone ever edits.
file(GLOB_RECURSE SHADER_SOURCE_FILES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/Shaders/*.frag
    ${CMAKE_SOURCE_DIR}/src/Shaders/*.vert
)

set(SHADER_GEN_DIR ${CMAKE_BINARY_DIR}/generated/ShaderCode)
file(MAKE_DIRECTORY ${SHADER_GEN_DIR})

set(GENERATED_SHADER_SOURCES)
foreach(SHADER_FILE ${SHADER_SOURCE_FILES})
    # The variable name embedded in ShaderCode.h always matches the shader's
    # file stem (objectFrag.frag -> objectFrag), so it can be derived here
    # instead of maintained as a separate list.
    get_filename_component(SHADER_VAR ${SHADER_FILE} NAME_WE)
    set(GENERATED_FILE ${SHADER_GEN_DIR}/${SHADER_VAR}.gen.cpp)
    add_custom_command(
        OUTPUT ${GENERATED_FILE}
        COMMAND ${CMAKE_COMMAND}
            -DIN_FILE=${SHADER_FILE}
            -DVAR_NAME=${SHADER_VAR}
            -DOUT_FILE=${GENERATED_FILE}
            -P ${CMAKE_SOURCE_DIR}/cmake/EmbedShader.cmake
        DEPENDS ${SHADER_FILE} ${CMAKE_SOURCE_DIR}/cmake/EmbedShader.cmake
        COMMENT "Embedding shader ${SHADER_VAR}"
        VERBATIM
    )
    list(APPEND GENERATED_SHADER_SOURCES ${GENERATED_FILE})
endforeach()

add_executable(renderer
    include/glad.c
    ${RENDERER_SOURCES}
    ${GENERATED_SHADER_SOURCES}
)

target_include_directories(renderer PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# -O3 in Release comes from the build type; nothing extra needed here.

# --- Profiling build (RelWithDebInfo) --------------------------------------
# CMake's stock RelWithDebInfo is "-O2 -g -DNDEBUG" - a different optimizer
# tier than Release. Override it to "-O3 -g" so profiling (VTune, perf, etc.)
# runs against the same codegen that actually ships, just with symbols kept.
# -fno-omit-frame-pointer makes hardware-event-sampling stack unwinding (VTune
# hotspots/microarch analysis, perf) reliable; it's a no-op for correctness.
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O3 -g -fno-omit-frame-pointer")

# --- Native CPU tuning ----------------------------------------------------
# -march=native tunes codegen for the exact CPU doing the build, so it's only
# appropriate for local dev builds, not binaries meant to run elsewhere.
# Guarded with a compile check since not every compiler/target accepts it.
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-march=native" RZ_HAS_MARCH_NATIVE)
if(RZ_HAS_MARCH_NATIVE)
    target_compile_options(renderer PRIVATE -march=native)
else()
    message(STATUS "-march=native not supported by this compiler - skipping.")
endif()

# --- GLFW ---------------------------------------------------------------------
# Prefer the package config GLFW ships (Homebrew/apt/vcpkg all provide it),
# fall back to pkg-config.
find_package(glfw3 QUIET)
if(glfw3_FOUND)
    target_link_libraries(renderer PRIVATE glfw)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    target_include_directories(renderer PRIVATE ${GLFW_INCLUDE_DIRS})
    target_link_libraries(renderer PRIVATE ${GLFW_LIBRARIES})
    target_link_directories(renderer PRIVATE ${GLFW_LIBRARY_DIRS})
endif()

# --- OpenMP (optional) --------------------------------------------------------
# Used by the matrix code; the renderer links it whenever it's available.
find_package(OpenMP QUIET)
if(NOT OpenMP_CXX_FOUND AND APPLE)
    # Apple clang ships without OpenMP support out of the box; wire up the
    # Homebrew libomp keg by hand (brew install libomp) and retry.
    execute_process(COMMAND brew --prefix libomp
        OUTPUT_VARIABLE _RZ_OMP_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
    if(_RZ_OMP_PREFIX AND EXISTS "${_RZ_OMP_PREFIX}/include/omp.h")
        set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I${_RZ_OMP_PREFIX}/include")
        set(OpenMP_CXX_LIB_NAMES "omp")
        set(OpenMP_omp_LIBRARY "${_RZ_OMP_PREFIX}/lib/libomp.dylib")
        find_package(OpenMP QUIET)
    endif()
endif()
if(OpenMP_CXX_FOUND)
    target_link_libraries(renderer PRIVATE OpenMP::OpenMP_CXX)
else()
    message(STATUS "OpenMP not found - building without it.")
endif()

find_package(TBB REQUIRED)
target_link_libraries(renderer PRIVATE TBB::tbb)

# --- nlohmann/json --------------------------------------------------------
# Prefer a system-installed package (Homebrew: nlohmann-json, apt:
# nlohmann-json3-dev, vcpkg: nlohmann-json); fall back to fetching it at
# configure time so a fresh checkout still builds without extra setup.
find_package(nlohmann_json 3.11 QUIET)
if(NOT nlohmann_json_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()
target_link_libraries(renderer PRIVATE nlohmann_json::nlohmann_json)

# --- Platform libraries -------------------------------------------------------
if(APPLE)
    # glad loads GL symbols at runtime via dlopen; GLFW needs these frameworks.
    target_link_libraries(renderer PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework OpenGL"
        ${CMAKE_DL_LIBS}
    )

    # experimental and its libdispatch backend lives in libc++experimental.a.
    # For std::execution::par on macOS. libc++'s Parallel STL is still
    # -fexperimental-library enables the headers at compile time AND, when passed
    # to the linker, pulls in libc++experimental.a so the PSTL backend symbols
    # (__libdispatch::__dispatch_apply, __partition_chunks, ...) resolve.
    target_compile_options(renderer PRIVATE -fexperimental-library)
    target_link_options(renderer PRIVATE -fexperimental-library)
else()
    target_link_libraries(renderer PRIVATE ${CMAKE_DL_LIBS})
endif()

