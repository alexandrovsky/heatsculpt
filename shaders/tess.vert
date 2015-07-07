#version 410 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 position;
in vec3 color;

out vec4 color_vs;

void main(void)
{
    color_vs = vec4(color, 1.0);
    
    mat4 mvp = projection * view * model;
    gl_Position = mvp * vec4(position, 1.0);
}
