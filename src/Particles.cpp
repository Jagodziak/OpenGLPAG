#include "Particles.hpp"
#include <random>
#include <glm/ext/matrix_transform.hpp>

Particles::Particles(Model* particleModel, Shader* shader, Transform* root, int maxParticles, int spawnEveryXFrames)
{
	this->shader = shader;
	this->particleModel = particleModel;
	this->spawnEveryXFrames = spawnEveryXFrames;
	this->maxParticles = maxParticles;
	this->root = root;
	particleTransforms.reserve(maxParticles);
	spawnedParticles.reserve(maxParticles);
}

void Particles::UpdateAndRender()
{
	std::default_random_engine generator(currentFrame);
	// Spawning
	if (currentFrame % spawnEveryXFrames == 0 && maxParticles > spawnedParticles.size())
	{
		Particle newParticle;

		std::uniform_int_distribution<int> spawn(1, 10);
		spawnEveryXFrames = spawn(generator);

		std::uniform_real_distribution<float> rotation(-0.05f, 0.05f);
		std::uniform_real_distribution<float> scale(0.005f, 0.02f);
		std::uniform_real_distribution<float> translation(0.5f, 2.0f);
		std::uniform_real_distribution<float> translation2(0.01f, 0.1f);
		std::uniform_int_distribution<int> lifetime(lifetimeRandRange.x, lifetimeRandRange.y);

		newParticle.maxLifetime = lifetime(generator);
		newParticle.movementSpeed = glm::vec3(translation2(generator), translation(generator), translation2(generator));
		newParticle.rotationSpeed = { rotation(generator), rotation(generator), rotation(generator) };
		newParticle.scaleSpeed = glm::vec3(scale(generator));

		spawnedParticles.push_back(newParticle);

		Transform newParticleTransform;
		root->addChild(&newParticleTransform);
		particleTransforms.push_back(newParticleTransform);
	}

	// Updating
	for (size_t i = 0; i < spawnedParticles.size(); i++)
	{
		Particle& particle = spawnedParticles[i];
		if (particle.currentLifetime > particle.maxLifetime)
		{
			// Deleting particle
			spawnedParticles.erase(spawnedParticles.begin() + i);
			particleTransforms.erase(particleTransforms.begin() + i);
			i--;
			continue;
		}
		particle.currentLifetime++;

		particle.position += particle.movementSpeed;
		particle.rotation += particle.rotationSpeed;
		particle.scale += particle.scaleSpeed;

		particleTransforms[i].reset();
		particleTransforms[i].move(particle.position);
		particleTransforms[i].rotate(particle.rotation);
		particleTransforms[i].scale(particle.scale);
		//particleTransforms[i].worldTransform = glm::translate(particleTransforms[i].worldTransform, particle.movementSpeed);
		//particleTransforms[i].recalculate();
	}

	// Rendering
	particleModel->updateInstanceMatrices(&particleTransforms);
	particleModel->draw(*shader);

	currentFrame++;
}
