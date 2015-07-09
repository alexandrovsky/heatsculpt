#version 400 core


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


uniform vec3 mousePosition;

in vec3 position;
in vec3 color;

out vec4 vColor;

void main() {
    mat4 mvp = projection * view * model;
    vec3 p = position;
    gl_Position = mvp * vec4(mousePosition, 1.0);
    vColor =  vec4(color, 1.0);
}

