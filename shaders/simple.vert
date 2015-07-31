#version 400 core


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0)in vec3 position;
layout(location = 1)in vec3 color;

out vec3 vColor;

void main() {
    
    vColor = color;
    mat4 mvp = projection * view * model;

    gl_Position = mvp * vec4(position, 1.0);

}

