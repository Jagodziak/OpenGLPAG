#pragma once

#include "Cubemap.hpp"
#include "Shader.hpp"

class Skybox
{
public:
	Skybox(std::string cubemapPath);

	void draw(Shader& shader);

	Cubemap cubemap;
private:
	GLuint VAO, VBO;
	static const float skyboxVertices[];
};

