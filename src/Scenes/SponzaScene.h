#ifndef SPONZA_SCENE_H
#define SPONZA_SCENE_H

#include "Objects/Drawable.h"
#include "System/Window.h"

std::shared_ptr<ComplexDrawable> loadSponza();
void setUpSponzaLighting(Window& win);
void updateSponzaScene(double t, Window& win);

#endif
