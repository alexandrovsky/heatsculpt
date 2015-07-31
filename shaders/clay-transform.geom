#version 400 core

layout(points, invocations = 1) in;
layout(points, max_vertices = 2) out;



in vec3 vPosition[1];
in vec3 vVelocity[1];


out vec3 outposition;
out vec3 outvelocity;


void main(){
    
    outposition = vPosition[0];
    outvelocity = vec3(0.0,0.0,1.0);
    
    EmitVertex();
    
    
//    outposition = vPosition[0];
//    outvelocity = vec3(1.0,0.0,0.0);
//    
//    EmitVertex();
    
    
    EndPrimitive();
    
}