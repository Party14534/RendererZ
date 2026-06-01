cmake_minimum_required(VERSION 3.16)

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

add_executable(renderer
    include/glad.c
    ${RENDERER_SOURCES}
)

target_include_directories(renderer PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# -O3 in Release comes from the build type; nothing extra needed here.

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
else()
    target_link_libraries(renderer PRIVATE ${CMAKE_DL_LIBS})
endif()
