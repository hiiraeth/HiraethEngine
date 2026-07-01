#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec2 TextureCoord;
out vec3 FragPos;
out vec3 Normal;

void main() {
    vec4 vertexPosition = vec4(aPos, 1.0);

    gl_Position  = projection * view * model * vertexPosition;
    TextureCoord = aTex;
    Normal       = mat3(transpose(inverse(model))) * aNormal;
    FragPos      = vec3(model * vertexPosition);
}
