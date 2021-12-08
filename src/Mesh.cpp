#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<glm::vec3>* instanceOffsets)
{
    this->instanceOffsets = instanceOffsets;
    indexCount = indices.size(); 

	// Generate and bind VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate, bind and fill EBO
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Generate, bind and fill VBO
    glGenBuffers(1, &VBO); 

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    if (instanceOffsets != nullptr)
    {
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * instanceOffsets->size(), instanceOffsets->data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(3, 1);
    }

    // Bind to null for safety
    glBindVertexArray(0);
}

void Mesh::draw(Shader& shader, bool drawAsLine)
{
    shader.use();
    glBindVertexArray(VAO);

    GLenum drawingMode = drawAsLine ? GL_LINES : GL_TRIANGLES;

    if (instanceOffsets == nullptr)
    {
        glDrawElements(drawingMode, indexCount, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawElementsInstanced(drawingMode, indexCount, GL_UNSIGNED_INT, 0, instanceOffsets->size());
    }
    glBindVertexArray(0);
}
