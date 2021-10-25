#version 330 core // To samo co w vertex shaderze
out vec4 FragColor; // Kolor piksela który zostanie wpisany na ekran

in vec2 Texcoord;

uniform vec4 color;
uniform sampler2D ourTexture; // sampler2D - typ który pozwala nam uzyskaæ dane tekstury

void main()
{
    FragColor = texture(ourTexture, Texcoord) * color; //zmienia kolor tekstury
}