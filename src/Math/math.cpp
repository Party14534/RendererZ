#include "math.h"

float radians(const float degrees) {
    return degrees * (PI / 180);
}

float degrees(const float radians) {
    return radians * (180 / PI);
}
