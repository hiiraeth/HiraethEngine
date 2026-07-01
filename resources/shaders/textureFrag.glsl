#version 330 core

in vec2 TextureCoord;
in vec3 aNormal;
in vec3 FragPos;

out vec4 FragColor;

// texture
uniform sampler2D textureSampler;

void main() {
    FragColor = texture(textureSampler, TextureCoord);
}
