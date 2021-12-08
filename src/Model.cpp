#include "Model.hpp"

#include <iostream>

Model::Model(std::string modelPath, bool drawAsLine, std::vector<glm::vec3>* instanceOffsets)
{
	this->drawAsLine = drawAsLine;
	this->isInstanced = isInstanced;
	this->instanceOffsets = instanceOffsets;
	loadModel(modelPath);
}

Model::Model()
{
	this->drawAsLine = false;
	meshes.push_back(Mesh());
}

void Model::generateCone(float coneRadius, float coneHeight, int coneSides)
{
	std::vector<Vertex> coneVertices;
	std::vector<unsigned int> coneIndices;

	Vertex tempVert; 	
	tempVert.pos = glm::vec3(0.0f, 0.0f, 0.0f);
	tempVert.norm = glm::vec3(0.0f, 0.0f, 0.0f);
	tempVert.texcoord = glm::vec2(0.0f, 0.0f);

	// The bottom center 0.0f 0.0f 0.0f vertex
	coneVertices.push_back(tempVert);

	// The top vertex
	tempVert.pos = glm::vec3(0.0f, coneHeight, 0.0f);
	coneVertices.push_back(tempVert);

	// The bottom circle vertices
	float incrementAngle = glm::radians(360.0f / coneSides);
	for (int i = 0; i <= coneSides; i++)
	{
		float x = coneRadius * glm::cos(incrementAngle * i);
		float z = coneRadius * glm::sin(incrementAngle * i);
		tempVert.pos = glm::vec3(x, 0.0f, z);
		coneVertices.push_back(tempVert);
	}

	// Mesh indices
	for (int i = 0; i <= coneSides; i++)
	{
		// Bottom triangle
		coneIndices.push_back(0);
		coneIndices.push_back(i + 3);
		coneIndices.push_back(i + 2);
		// Side triangle
		coneIndices.push_back(1);
		coneIndices.push_back(i + 2);
		coneIndices.push_back(i + 3);
	}

	meshes[0] = Mesh(coneVertices, coneIndices);
}

void Model::draw(Shader& shader)
{
	shader.setMat4("model", modelTransform.getWorldTransform());

	glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
	glBindTexture(GL_TEXTURE_2D, texture.getTextureId());

	shader.setInt("ourTexture", 0); // Bind texture unit 0 to ourTexture

	// For each mesh in meshes
	for (auto mesh : meshes)
	{
		mesh.draw(shader, drawAsLine);
	}
}

void Model::loadModel(std::string modelPath)
{
	Assimp::Importer importer;
	// Loading model using Assimp library
	const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

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
	//wyci¹ganie wierzchó³ków z aiMesh do Mesh
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

	return Mesh(vertices, indices, instanceOffsets);
}
