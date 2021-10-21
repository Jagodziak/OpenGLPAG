#version 330 core // To samo co w vertex shaderze
out vec4 FragColor; // Kolor piksela który zostanie wpisany na ekran

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // Wpisujemy kolor do zmiennej FragColor (rgba)
}