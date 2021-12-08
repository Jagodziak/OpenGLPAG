#version 330 core 
out vec4 FragColor; 

in vec2 Texcoord;

uniform sampler2D ourTexture; 
uniform vec3 ambient;

void main()
{
    FragColor = texture(ourTexture, Texcoord) * vec4(ambient, 1.0); 
}