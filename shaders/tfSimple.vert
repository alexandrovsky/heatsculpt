#version 330

in vec3 inposition;
out vec3 outposition;


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





vec3 qtransform( vec4 q, vec3 v ){
    return v + 2.0*cross(cross(v, q.xyz ) + q.w*v, q.xyz);
}

void main() {
    
    vec3 p = inposition;
//    p.x *= 0.9;
//    p.y *= 0.9;
//    p.z *= 0.9;
    
    
    p *= 0.999;
    
    outposition = p;
    
    
////    mat4 rot = rotationMatrix(vec3(0.0, 1.0, 0.0), 5);
////    
////    vec4 p = rot * vec4(inposition, 1);
////    outposition = p.xyz;
//    
//    vec4 q = vec4(0.0, 1.0, 0.0, 1.0);
//    vec3 v = inposition;
//    
//    
//    
//    outposition = qtransform(q, v);
    
}
