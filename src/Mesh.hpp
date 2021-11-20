#pragma once
//klasa mesh zapewnia abstrakcjê nad zbiorem wierzcho³ków i ich ideksów. Zapewnia wygodne przekazywanie ich do pamiêci karty graficznej oraz udostêpnia 
//metodê pozwalaj¹ca na narysowanie siatki odpowiednim shaderem. 
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.hpp"

struct Vertex
{
	glm::vec3 pos; // pozycja wierzcho³ka
	glm::vec3 norm; // wektor normalny
	glm::vec2 texcoord; // ufa³ka
};

class Mesh
{
public:
	Mesh() = default; //konstruktor bezargumentowy domyœlny. daje mi to to ¿e moge
					  //utworzyæ obiekt klasy mesh bez podawania argumentów i nie muszê pisaæ implementacji konstruktora (CONE)
					  //tworzê mesh który jest pusty po to by stworzyæ Cone
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices); //wektor wierzcho³ków i wektor indeksów(przepisów na trójk¹ty)

	void draw(Shader& shader, bool drawAsLine = false); // mówimy funkcji draw którym shaderem bêdziemy rysowaC. drawasline od rysowania orbit

private:
	int indexCount; //iloœæ indeksów potrzebna do wywo³ania jakiejœ funkcji w draw
	GLuint VAO; // Vertex array object
	GLuint VBO; // Vertex buffer object (holds vertices)
	GLuint EBO; // Element buffer object (holds indices)
};