#version 410 core



in vec3 position;
in vec3 color;

out vec4 vColor;

out vec3 vPosition;

void main(void)
{
    vColor = vec4(color, 1.0);
    vPosition = position;

    //gl_Position = vec4(position, 1.0);
    }
