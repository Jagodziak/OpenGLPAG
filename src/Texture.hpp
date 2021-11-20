#pragma once
#include <string>
#include <glad/glad.h>
//odpowiedzialnoscia klasy texture jest ³adowanie tekstury z pliku, przekazywanie jej do pamiêci karty graficznej oraz przechowywanie jej ID
class Texture
{
public:
	Texture();

	void load(std::string texturePath); //metoda ³aduj¹ca texture z pliku przy pomocy biblioteki stb_image

	GLuint getTextureId();

private:
	GLuint textureID;
};