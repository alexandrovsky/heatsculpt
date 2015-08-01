#version 400 core
layout(lines) in;
layout(line_strip, max_vertices=2) out;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 origin;
uniform vec3 destination;

out vec4 gColor;


void main(void){
    
    mat4 mvp = projection * view * model;

    gl_Position = mvp * vec4(origin, 1.0);
    gColor = vec4(0.0, 1.0, 0.0, 1.0);
    EmitVertex();

    gl_Position = mvp * vec4(destination, 1.0);
    gColor = vec4(0.0, 0.0, 1.0, 1.0);
    EmitVertex();

}
