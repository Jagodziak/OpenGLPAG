#pragma once
#include <string>
#include <glad/glad.h>

class Texture
{
public:
	Texture();

	void load(std::string texturePath); 

	GLuint getTextureId();

private:
	GLuint textureID;
};