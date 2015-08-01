#version 400 core



in vec3 inposition;
in vec3 invelocity;


out Vertex
{
    vec3 position;
    vec3 velocity;

} vertex;






void main() {
    
    vertex.position = inposition;
    vertex.velocity = invelocity;
}