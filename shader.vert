#version 410 core

layout (location = 0) in vec3 vert;
layout (location = 1) in vec2 tex;
out vec2 passTex;

uniform mat4 mvpMatrix;

void main() {
    gl_Position = mvpMatrix * vec4(vert, 1.0);
    passTex = tex;
}
