#version 330 core 
out vec4 FragColor; 

in vec2 Texcoord;

uniform vec4 color;
uniform sampler2D ourTexture; 

void main()
{
    FragColor = texture(ourTexture, Texcoord) * color; 
}