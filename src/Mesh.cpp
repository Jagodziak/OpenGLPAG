#include "Mesh.hpp"
#include <Transform.hpp>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Transform>* instanceOffsets)
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

    glGenBuffers(1, &instanceVBO);
    updateInstanceMatrices();

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    std::size_t vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

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

void Mesh::updateInstanceMatrices()
{
    if (instanceOffsets != nullptr)
    {
        glm::mat4* matrices = new glm::mat4[instanceOffsets->size()];
        for (size_t i = 0; i < instanceOffsets->size(); i++)
        {
            matrices[i] = (*instanceOffsets)[i].getWorldTransform();
        }

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instanceOffsets->size(), matrices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        delete matrices;
    }
    else
    {
        // This is a dirty dirty hack I am really sorry for this :<
        glm::mat4 identity(1.0f);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &identity, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
