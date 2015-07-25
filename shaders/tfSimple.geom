#version 400

#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable

layout(points) in;
layout(points, max_vertices = 1) out;

in vec3[] vPosition;

out vec3 outposition;

void main() {
    vec3 pos = vPosition[0];
    outposition = pos;
    EmitVertex();
    EndPrimitive();
}