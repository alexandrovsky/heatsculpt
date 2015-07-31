#version 400 core


in vec3 vColor;
out vec4 outColor;
void main() {
    outColor = vec4(vColor, 1.0);//vec4(0.8, 0.2, 0.2, 1.0);
    
}

