#include "Model.hpp"

#include <iostream>

Model::Model(std::string modelPath)
{
	loadModel(modelPath);
}

void Model::draw(Shader& shader)
{
	// For each mesh in meshes
	for (auto mesh : meshes)
	{
		mesh.draw(shader);
	}
}

void Model::loadModel(std::string modelPath)
{
	Assimp::Importer importer;
	// Loading model using Assimp library
	const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

	// Cheking for file loading errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	// Get rid of the file name from the path because of reasons
	directory = modelPath.substr(0, modelPath.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process vertex positions, normals and texture coordinates
		// Vec3 for holding position read from assimp mesh
		glm::vec3 assimpPos;
		assimpPos.x = mesh->mVertices[i].x;
		assimpPos.y = mesh->mVertices[i].y;
		assimpPos.z = mesh->mVertices[i].z;
		vertex.pos = assimpPos;

		glm::vec3 assimpNorm;
		assimpNorm.x = mesh->mNormals[i].x;
		assimpNorm.y = mesh->mNormals[i].y;
		assimpNorm.z = mesh->mNormals[i].z;
		vertex.norm = assimpNorm;

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 assimpTexcoord;
			assimpTexcoord.x = mesh->mTextureCoords[0][i].x;
			assimpTexcoord.y = mesh->mTextureCoords[0][i].y;
			vertex.texcoord = assimpTexcoord;
		}
		else
		{
			vertex.texcoord = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// Reading indices from assimp structures
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	return Mesh(vertices, indices);
}
