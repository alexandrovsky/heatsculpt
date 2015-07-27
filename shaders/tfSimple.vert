#version 330

in vec3 inPosition;
in vec3 inColor;

out vec3 vPosition;
out vec3 vColor;





vec3 qtransform( vec4 q, vec3 v ){
    return v + 2.0*cross(cross(v, q.xyz ) + q.w*v, q.xyz);
}

void main() {
    
    vec3 p = inPosition;
    p *= 0.999;
    vPosition = p;
    
    vec3 c = inColor;
    
    c *= 0.999;
    vColor = c;
    
    
//    vec4 q = vec4(0.0, 1.0, 0.0, 1.0);
//    vec3 v = inposition;
//    
//    
//    
//    outposition =  qtransform(q, v);
    
}
