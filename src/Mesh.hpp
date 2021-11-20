#pragma once
//klasa mesh zapewnia abstrakcj� nad zbiorem wierzcho�k�w i ich ideks�w. Zapewnia wygodne przekazywanie ich do pami�ci karty graficznej oraz udost�pnia 
//metod� pozwalaj�ca na narysowanie siatki odpowiednim shaderem. 
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.hpp"

struct Vertex
{
	glm::vec3 pos; // pozycja wierzcho�ka
	glm::vec3 norm; // wektor normalny
	glm::vec2 texcoord; // ufa�ka
};

class Mesh
{
public:
	Mesh() = default; //konstruktor bezargumentowy domy�lny. daje mi to to �e moge
					  //utworzy� obiekt klasy mesh bez podawania argument�w i nie musz� pisa� implementacji konstruktora (CONE)
					  //tworz� mesh kt�ry jest pusty po to by stworzy� Cone
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices); //wektor wierzcho�k�w i wektor indeks�w(przepis�w na tr�jk�ty)

	void draw(Shader& shader, bool drawAsLine = false); // m�wimy funkcji draw kt�rym shaderem b�dziemy rysowaC. drawasline od rysowania orbit

private:
	int indexCount; //ilo�� indeks�w potrzebna do wywo�ania jakiej� funkcji w draw
	GLuint VAO; // Vertex array object
	GLuint VBO; // Vertex buffer object (holds vertices)
	GLuint EBO; // Element buffer object (holds indices)
};