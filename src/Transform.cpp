#include "Transform.hpp"
#include <glm/ext/matrix_transform.hpp>

Transform::Transform()
{
	this->localTransform = glm::mat4(1.0f); //ustawia macierze jednostkowe ¿eby transformacja by³a pusta
	this->worldTransform = glm::mat4(1.0f);
}

Transform::Transform(glm::mat4 localTransform)
{
	this->localTransform = localTransform;
	this->worldTransform = localTransform;
}

void Transform::addChild(Transform* child)
{
	children.push_back(child); //dodajemy wektora children wskaŸnik na dziecko
	child->recalculate(worldTransform); //dla dodanego dziecka pzeliczamy jego world transform
}

void Transform::recalculate(glm::mat4 parentTransform)//macierz worldTransform (tutaj nazwana parentTransform) rodzica jest mno¿ona z LocalTransform
													  //recalculate jest wywolywane rekurencyjnie dla wszystkich dzieci.
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
	recalculate();
}

void Transform::simulate() //obraca wokó³ w³asnej osi wszystkie dzieci którem amj¹ jakiœ rotation speed, ale jeszcze nie recalculuje tego
{
	if (rotationSpeed > 0.0001f || rotationSpeed < -0.0001f)
	{
		localTransform = glm::rotate(localTransform, rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	for (int i = 0; i < children.size(); i++)
	{
		children[i]->simulate();
	}
}

void Transform::move(glm::vec3 value)
{
	localTransform = glm::translate(localTransform, value);
	recalculate();
}

void Transform::rotate(glm::vec3 value) // mo¿e wyst¹piæ gimbal lock
{
	localTransform = glm::rotate(localTransform, value.x, glm::vec3(1.0f, 0.0f, 0.0f));
	localTransform = glm::rotate(localTransform, value.y, glm::vec3(0.0f, 1.0f, 0.0f));
	localTransform = glm::rotate(localTransform, value.z, glm::vec3(0.0f, 0.0f, 1.0f));
	recalculate();
}

void Transform::scale(glm::vec3 value)
{
	localTransform = glm::scale(localTransform, value);
	recalculate();
}

glm::mat4 Transform::getWorldTransform()
{
	return worldTransform;
}
