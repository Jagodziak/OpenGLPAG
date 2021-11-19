#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.hpp"

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 texcoord;
};

class Mesh
{
public:
	Mesh() = default;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

	void draw(Shader& shader, bool drawAsLine = false);

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

private:
	GLuint VAO; // Vertex array object
	GLuint VBO; // Vertex buffer object (holds vertices)
	GLuint EBO; // Element buffer object (holds indices)
};