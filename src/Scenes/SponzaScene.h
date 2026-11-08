#ifndef SPONZA_SCENE_H
#define SPONZA_SCENE_H

#include "../Objects/Drawable.h"
#include "../System/Window.h"

shared<ComplexDrawable> loadSponza();
void setUpSponzaLighting(Window& win);
void updateSponzaScene(double t, Window& win);

#endif
