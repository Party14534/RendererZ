#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model_z;
uniform mat4 view_z;
uniform mat4 projection_z;

void main()
{
    gl_Position = projection_z * view_z * model_z * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(model_z))) * aNormal;
    FragPos = vec3(model_z * vec4(aPos, 1.0));
}
