#version 410 core

layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 3) out;


in vec3 tePosition[3];
in vec4 teColor[3];
in vec3 tePatchDistance[3];
out vec3 gFacetNormal;
out vec3 gPatchDistance;
out vec3 gTriDistance;
out vec4 gColor;


void main(void)
{

    vec3 A = tePosition[2] - tePosition[0];
    vec3 B = tePosition[1] - tePosition[0];
    gFacetNormal = normalize(cross(A, B));
    
    gPatchDistance = tePatchDistance[0];
    gColor = teColor[0];
    gTriDistance = vec3(1, 0, 0);
    gl_Position = gl_in[0].gl_Position; EmitVertex();
    
    gPatchDistance = tePatchDistance[1];
    gColor = teColor[1];
    gTriDistance = vec3(0, 1, 0);
    gl_Position = gl_in[1].gl_Position; EmitVertex();
    
    gPatchDistance = tePatchDistance[2];
    gColor = teColor[2];
    gTriDistance = vec3(0, 0, 1);
    gl_Position = gl_in[2].gl_Position; EmitVertex();
    
    EndPrimitive();
}


