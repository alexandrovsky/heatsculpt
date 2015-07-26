#version 330
out vec4 outColor;

in vec3 vPosition;
in vec3 vColor;

void main() {
    vec3 np = normalize(vPosition);
    outColor = vec4(vColor, 1.0);
//    outColor = vec4(0.8, 0.2, 0.2, 1.0);
}