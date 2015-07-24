#version 400 core


//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

in vec3 position;
//in vec3 color;

out vec3 outposition;

void main() {
    
    vec3 p = position;
    
    //gl_Position = vec4(position, 1.0);
    outposition = p;
}
