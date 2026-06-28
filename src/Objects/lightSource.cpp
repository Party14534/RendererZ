#include "LightSource.h"

LightSource::LightSource(Vec3 _pos) : pos(_pos) { }

void LightSource::setColor(Color c) { color = c; }
Color LightSource::getColor() const { return color; }

void LightSource::setPos(Vec3 v) { pos = v; }
Vec3 LightSource::getPos() const { return pos; }

/*
 * Dir Light
 */
DirLight::DirLight(Vec3 _pos, DirLightProperties _prop) : properties(_prop), LightSource(_pos) { }


void DirLight::setDir(Vec3 v) { pos = v; }
Vec3 DirLight::getDir() const { return pos; }

/*
 * Point Light
 */
PointLight::PointLight() :
    properties(PointLightProperties()), LightSource(Vec3()) { }
PointLight::PointLight(Vec3 _pos, PointLightProperties _prop) :
    properties(_prop), LightSource(_pos) { }
