#include "Input.h"

Mouse::Mouse(Vec2 _pos) : pos(_pos) {}

Mouse::Mouse(float _x, float _y) : pos(Vec2(_x, _y)) {}

Mouse::Mouse(double _x, double _y) : pos(Vec2(_x, _y)) {}
