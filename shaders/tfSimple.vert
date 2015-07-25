#version 400 core


//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

in vec3 position;
//in vec3 color;

out vec3 vPosition;

void main() {
    vPosition = position;
}
