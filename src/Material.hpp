#pragma once
#include <Shader.hpp>
#include <Texture.hpp>
#include <vector>

class Material
{
public:
	Material(Shader& shader);

	void use();

	void setInt(int val);
	void setFloat(float val);

private:
	Shader& shader;

	std::vector<int> ints;
	std::vector<float> floats;
	std::vector<glm::vec3> vec3s;
	std::vector<glm::vec4> vec4s;
	std::vector<glm::mat4> mat4s;
	std::vector<Texture> textures;
};