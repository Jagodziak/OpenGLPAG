#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include <Transform.hpp>

class Model
{
public:
	Model(std::string modelPath);

	void draw(Shader& shader);
	Transform modelTransform;

private:
	std::vector<Mesh> meshes; // Collection for storing meshes

	std::string directory; // From which (relative) directory the model is sourced

	void loadModel(std::string modelPath);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};