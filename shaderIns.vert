#version 410 core

layout (location = 0) in vec3 vert;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 pos;
out vec2 passTex;

uniform mat4 mvpMatrix;

void main() {
    mat4 matrix = mat4(1.0, 0.0, 0.0, 0.0,
                       0.0, 1.0, 0.0, 0.0,
                       0.0, 0.0, 1.0, 0.0,
                       pos.x, pos.y, pos.z, 1.0);
    gl_Position = mvpMatrix * matrix * vec4(vert, 1.0);
    passTex = tex;
}
