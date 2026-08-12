#include "SkyBox.h"
#include "../Objects/Texture.h"
#include "../Shaders/shaders.h"
#include <memory>

SkyBox::SkyBox(std::vector<std::string> textures) :
    map(textures),
    Drawable(Mesh::skyBox()) { }

void SkyBox::draw(std::shared_ptr<Shader> defaultShader) {
    defaultShader->use();
    defaultShader->setMat4(SHADER_MODEL_SET_UNIFORM, transform.GetModelMat());
    defaultShader->setInt("skyBox_z", SKYBOX_TEXTURE_UNIT); 
    map.bind(SKYBOX_TEXTURE_UNIT);
    mesh->draw();
}
