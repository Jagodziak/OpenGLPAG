#pragma once
#include <vector>
#include <glm/glm.hpp>

class Transform
{
public:
	Transform();
	Transform(glm::mat4 localTransform);

	void addChild(Transform* child);
	void recalculate(glm::mat4 parentTransform);

	void reset();
	void move(glm::vec3 value);
	void rotate(glm::vec3 value);
	void scale(glm::vec3 value);

	glm::mat4 getWorldTransform();

	float rotationSpeed = 0.0f;

private:
	std::vector<Transform*> children;

	glm::mat4 localTransform;
	glm::mat4 worldTransform;
};