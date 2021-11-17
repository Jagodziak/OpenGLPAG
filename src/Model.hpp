#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"

class Model
{
public:
	Model(std::string modelPath);

	void draw(Shader& shader);

private:
	glm::mat4 modelMatrix; // Transforms our model from local to world space
	std::vector<Mesh> meshes; // Collection for storing meshes

	std::string directory; // From which (relative) directory the model is sourced

	void loadModel(std::string modelPath);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};