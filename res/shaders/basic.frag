#version 330 core // To samo co w vertex shaderze
out vec4 FragColor; // Kolor piksela kt�ry zostanie wpisany na ekran

in vec2 Texcoord;

uniform vec4 color;
uniform sampler2D ourTexture; // sampler2D - typ kt�ry pozwala nam uzyska� dane tekstury

void main()
{
    FragColor = texture(ourTexture, Texcoord) * color; //zmienia kolor tekstury
}