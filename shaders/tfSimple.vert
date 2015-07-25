//#version 400 core
//
//
////uniform mat4 model;
////uniform mat4 view;
////uniform mat4 projection;
//
//in vec3 position;
////in vec3 color;
//
//out vec3 vPosition;
//
//void main() {
//    vPosition = position;
//}


#version 330

layout(location = 0) in vec2 position;

out vec2 tPosition;

void main(){
    tPosition = position;
}