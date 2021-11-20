#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    indexCount = indices.size(); //wyci¹gamy sobie iloœæ indexów

	// Generate and bind VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate, bind and fill EBO
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Generate, bind and fill VBO
    glGenBuffers(1, &VBO); //funkcja tworz¹ca bufor na karcie graficznej i zapisuj¹ca jego ID w zmiennej VBO

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //przekazujemy dane do wczesniej stworzonego bufora
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Ustawienie wskaŸnika atrybutu
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // aktywowanie attribute pointera o indexie 0

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Bind to null for safety
    glBindVertexArray(0);
}

void Mesh::draw(Shader& shader, bool drawAsLine)
{
    shader.use();
    glBindVertexArray(VAO);
    if (drawAsLine)
    {
        glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}
