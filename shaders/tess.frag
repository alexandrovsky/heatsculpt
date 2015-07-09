#version 410 core

layout(location = 0, index = 0) out vec4 fragColor;

//in vec4 color_gs;

//out vec3 gFacetNormal;

uniform vec3 lightPosition;
uniform vec3 ambientMaterial;
uniform vec3 diffuseMaterial;


in vec3 gFacetNormal;
in vec3 gTriDistance;
in vec3 gPatchDistance;

in vec4 gColor;

float amplify(float d, float scale, float offset)
{
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1 - exp2(-2*d*d);
    return d;
}


void main(void)
{
    vec3 N = normalize(gFacetNormal);
    vec3 L = lightPosition;
    
    float df = abs(dot(N, L));
    vec3 color = ambientMaterial + df * diffuseMaterial;
    
    
    float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
    float d2 = min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z);
    color = amplify(d1, 40, -0.5) * amplify(d2, 60, -0.5) * color;
    
    fragColor = gColor * vec4(color, 1.0); // vec4(1.0,1.0,1.0,1.0);
}