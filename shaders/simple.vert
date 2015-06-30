#version 400 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 Position;
in vec3 Color;

out vec3 vColor;


void main() {
    mat4 mvp = projection * view * model;
    gl_Position = mvp * vec4(Position, 1.0);
    vColor = Color;
}

