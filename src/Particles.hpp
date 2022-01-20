#pragma once
#include <Model.hpp>

struct Particle
{
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec3 movementSpeed;
	glm::vec3 rotationSpeed;
	glm::vec3 scaleSpeed;
	int maxLifetime;
	int currentLifetime = 0;
};

class Particles
{
public:
	Particles(Model* particleModel, Shader* shader, Transform* root, int maxParticles, int spawnEveryXFrames);

	void UpdateAndRender();

	glm::vec3 initialSpeed = glm::vec3(0.0f, 0.5f, 0.0f);
	glm::ivec2 lifetimeRandRange = glm::ivec2(40, 700);
	float randomRotationRate = 1.0f;

	Shader* shader;

private:
	Model* particleModel;
	Transform* root;

	int maxParticles = 100;
	int spawnEveryXFrames;
	int currentFrame = 0;
	std::vector<Particle> spawnedParticles;
	std::vector<Transform> particleTransforms;
};

