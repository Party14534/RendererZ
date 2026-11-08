#version 330 core

layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 TexCoords;

uniform mat4 projection_z;
uniform mat4 view_z;

void main() {
    TexCoords = aPos;
    vec4 pos = projection_z * view_z * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
