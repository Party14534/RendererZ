#ifndef MANY_LIGHTS_SCENE_H
#define MANY_LIGHTS_SCENE_H

#include "Objects/Scene.h"
#include "System/Window.h"

Scene testManyLightsScene();
void setUpManyPointLights(Window& win);
void updateManyLightsScene(double t, Window& win);

#endif
