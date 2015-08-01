#version 400 core

layout(points, invocations = 1) in;
layout(points, max_vertices = 2) out;


uniform float t;
uniform int click;

in Vertex
{
    vec3 position;
    vec3 velocity;
    
} vertex[1];


mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}


out vec3 outposition;
out vec3 outvelocity;


void main(){
    
    
    float speed = 0.01;
    float _t = (t * speed);
    vec3 axis = vec3(0.0, 1.0, 0.0);
    mat4 rot = rotationMatrix(axis, _t);
    //
    vec4 rotPos = rot * vec4(vertex[0].position, 1.0);
    
    outposition = rotPos.xyz;
    outvelocity = vertex[0].velocity;
    
    EmitVertex();
    EndPrimitive();
    
    if (click > 0) {
        
        axis = vec3(-1.0, 0.0, 0.0);
        rot = rotationMatrix(axis, _t*2);
        
        rotPos = rot * vec4(vertex[0].position, 1.0);
        
        outposition = rotPos.xyz;
        outvelocity = vec3(0.0, 0.0, 0.0);
        
        EmitVertex();
        EndPrimitive();
    }
    


    
}