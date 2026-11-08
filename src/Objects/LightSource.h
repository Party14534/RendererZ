#ifndef LIGHT_SOURCE
#define LIGHT_SOURCE

#include "../Shaders/Shaders.h"

class LightSource {
    public:
        LightSource(Vec3 _pos);
        void setColor(Color c);
        Color getColor() const;
        void setPos(Vec3 v);
        Vec3 getPos() const;

    protected:
        Vec3 pos;
        Color color;
};

class DirLight : public LightSource {
    public:
        DirLightProperties properties;

        DirLight(Vec3 _pos, DirLightProperties _prop);
        void setDir(Vec3 v);
        Vec3 getDir() const;
};

class PointLight : public LightSource {
    public:
        PointLightProperties properties;

        PointLight();
        PointLight(Vec3 _pos, PointLightProperties _prop);
};

#endif
