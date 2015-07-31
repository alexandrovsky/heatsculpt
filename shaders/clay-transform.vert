#version 400 core

uniform float t;

in vec3 inposition;
in vec3 invelocity;

out vec3 vPosition;
out vec3 vVelocity;



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


void main() {
    float speed = 0.01;
    float _t = (t * speed);
    vec3 axis = vec3(0.0, 1.0, 0.0);
    mat4 rot = rotationMatrix(axis, _t);
//
    vec4 rotPos = rot * vec4(inposition, 1.0);
    vPosition = rotPos.xyz;
    vVelocity = invelocity;
}