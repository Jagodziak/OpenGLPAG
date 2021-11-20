#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include <Transform.hpp>
#include <Texture.hpp>

class Model
{
public:
	Model(std::string modelPath, bool drawAsLine = false);
	Model();

	void generateCone(float coneRadius, float coneHeight, int coneSides);
	void draw(Shader& shader);

	Transform modelTransform;
	Texture texture;

private:
	std::vector<Mesh> meshes; // Collection for storing meshes
	std::string directory; // From which (relative) directory the model is sourced
	bool drawAsLine;

	void loadModel(std::string modelPath);
	void processNode(aiNode* node, const aiScene* scene); //przetwarza aiNode tak ¿eby mo¿na by³o wyci¹gn¹æ z niego aiMesh
	Mesh processMesh(aiMesh* mesh, const aiScene* scene); //przetwarza aiMesh tak ¿eby mo¿na by³o wyci¹gn¹æ z niego nasz mesh
};