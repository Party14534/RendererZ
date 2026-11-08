#ifndef MATERIAL_H
#define MATERIAL_H

#include "../Objects/Texture.h"
#include "../types.h"

struct ShaderProgram;

struct Material {
    public:
        Color color;
        Material(Color c = Color()) : color(c) {}
        virtual void setMaterial(shared<ShaderProgram> p) = 0;
};

/*
struct BlinnPhongMaterial : public Material {
    float ambient = 1.f;
    float diffuse = 1.f;
    float specular = 1.f;
    float shininess = 32.f;
    float reflectivity = 0.f;
    
    BlinnPhongMaterial() {}
    BlinnPhongMaterial(Color c, float a, float d,
            float s, float sh, float r) 
        : Material(c), ambient(a), diffuse(d), specular(s),
        shininess(sh), reflectivity(r) {}

    void setMaterial(ShaderProgram& p) override;
}; */

struct PBRMaterial : Material {
    shared<Texture> albedo, normal, metallicRoughness;
    float metallicFactor;
    float roughnessFactor;

    bool hasMRTexture;

    PBRMaterial(shared<Texture> a, shared<Texture> n,
            shared<Texture> mr,
            Color bcf, float mf, float rf)
        : albedo(a), normal(n), metallicRoughness(mr),
        metallicFactor(mf), roughnessFactor(rf), Material(bcf){}

    void setMaterial(shared<ShaderProgram> p) override;
};

shared<PBRMaterial> CreateDefaultMaterial();

#endif
