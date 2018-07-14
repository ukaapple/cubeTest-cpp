#version 410 core

in vec2 passTex;
out vec4 outColor;

uniform sampler2D textureSampler;

void main() {
    outColor = texture(textureSampler, passTex);
}
