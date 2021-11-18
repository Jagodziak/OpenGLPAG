#include "Transform.hpp"
#include <glm/ext/matrix_transform.hpp>

Transform::Transform()
{
	this->localTransform = glm::mat4(1.0f);
	this->worldTransform = glm::mat4(1.0f);
}

Transform::Transform(glm::mat4 localTransform)
{
	this->localTransform = localTransform;
	this->worldTransform = localTransform;
}

void Transform::addChild(Transform* child)
{
	children.push_back(child);
	child->recalculate(worldTransform);
}

void Transform::recalculate(glm::mat4 parentTransform)
{
	worldTransform = parentTransform * localTransform;
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->recalculate(worldTransform);
	}
}

void Transform::reset()
{
	localTransform = glm::mat4(1.0f);
	recalculate(glm::mat4(1.0f));
}

void Transform::move(glm::vec3 value)
{
	localTransform = glm::translate(localTransform, value);
	recalculate(glm::mat4(1.0f));
}

void Transform::rotate(glm::vec3 value)
{
	localTransform = glm::rotate(localTransform, value.x, glm::vec3(1.0f, 0.0f, 0.0f));
	localTransform = glm::rotate(localTransform, value.y, glm::vec3(0.0f, 1.0f, 0.0f));
	localTransform = glm::rotate(localTransform, value.z, glm::vec3(0.0f, 0.0f, 1.0f));
	recalculate(glm::mat4(1.0f));
}

void Transform::scale(glm::vec3 value)
{
	localTransform = glm::scale(localTransform, value);
	recalculate(glm::mat4(1.0f));
}

glm::mat4 Transform::getWorldTransform()
{
	return worldTransform;
}
