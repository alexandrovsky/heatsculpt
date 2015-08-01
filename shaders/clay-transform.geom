#version 400 core

layout(points, invocations = 1) in;
layout(points, max_vertices = 2) out;


uniform float t;
uniform int click;

uniform int interaction0_enabled;
uniform vec3 interaction0_pos;
uniform vec3 interaction0_normal;

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


mat4 translationMatrix(vec3 p)
{
    
    return mat4(1.0, 0.0, 0.0, p.x,
                0.0, 1.0, 0.0, p.y,
                0.0, 0.0, 1.0, p.z,
                0.0, 0.0, 0.0, 1.0);
}


out vec3 outposition;
out vec3 outvelocity;


void main(){
    

    vec3 in_pos = vertex[0].position;
    vec3 in_vel = vertex[0].velocity;

    int inter0Enabled = interaction0_enabled;
    vec3 inter0position = interaction0_pos;
    vec3 inter0normal = interaction0_normal;
    
    float speed = 0.125;
    float _t = (t * speed);
    vec3 axis = vec3(0.0, 1.0, 0.0);
    mat4 rot = rotationMatrix(axis, _t);
    //
    
    outposition = in_pos + in_vel;
    
    
    
    float dist = distance(inter0position, in_pos);
    float radius = 12.0;
    if (interaction0_enabled == 1
        && dist > 0.01
        && dist < radius) {
        vec3 direction = normalize(inter0position - in_pos);
//
//        mat4 transl = translationMatrix(direction);
//        vec4 translPos = transl * vec4(in_pos, 1.0);
//        
        
        outvelocity = direction * speed;
        
    }else{
        outvelocity = vec3(0); //in_pos * 0.5;
    }
    
    
    
    
    
    
    EmitVertex();
    EndPrimitive();
    
    if (click > 0) {
        
        axis = vec3(-1.0, 0.0, 0.0);
        rot = rotationMatrix(axis, _t*2);
        
        
        outposition = in_pos;
        outvelocity = vec3(0.0, 0.0, 0.0);
        
        
        //----
        
        
        EmitVertex();
        EndPrimitive();
    }
    


    
}