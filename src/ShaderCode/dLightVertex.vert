#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix_z;
uniform mat4 model_z;

void main() {
    gl_Position = lightSpaceMatrix_z * model_z * vec4(aPos, 1.);
}
