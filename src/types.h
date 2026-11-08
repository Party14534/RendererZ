#ifndef RENDERERZ_TYPES_H
#define RENDERERZ_TYPES_H

#include <cstdint>
#include <memory>

typedef int32_t i32;
typedef uint32_t u32;

// Templates
template <typename T>
using shared = std::shared_ptr<T>;

#endif
