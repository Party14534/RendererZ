#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skyBox_z;

void main() {
    FragColor = texture(skyBox_z, TexCoords);
}
