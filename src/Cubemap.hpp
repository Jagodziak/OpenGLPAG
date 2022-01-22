#pragma once
#include <string>
#include <glad/glad.h>

class Cubemap
{
public:
	Cubemap();

	void load(std::string cubemapPath);

	GLuint getCubemapId();

private:
	GLuint cubemapID;
};