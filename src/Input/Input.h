#ifndef INPUT_H
#define INPUT_H

#include "Math/math.h"

struct Mouse {
    Vec2 pos;

    Mouse(Vec2 _pos);
    Mouse(double x, double y);
    Mouse(float x, float y);
};

#endif
