#ifndef SKY_BOX_H
#define SKY_BOX_H

#include "Drawable.h"
#include "../Objects/Texture.h"
class SkyBox : Drawable {
    public:
        CubeMap map;

        SkyBox(std::vector<std::string> textures);

        void draw(std::shared_ptr<Shader> defaultShader);
        void init();
};

#endif
