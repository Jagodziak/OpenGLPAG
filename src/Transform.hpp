#pragma once
#include <vector>
#include <glm/glm.hpp>

//implementacja grafu sceny tak naprawde
//ka�dy transform jest przedstawieniem translacji rotacji i skali w przestrzeni 3d
//transformy u�o�one s� w hierarchi�; ka�dy transform posiada wska�niki na swoje dzieci.
//Je�li poruszymy rodzicem to porusza si� te� jego dziecko. W ten sam spos�b ofc.
//Funkcjonalno�� t� funkcjonalno�� zapewnia funkcja recalculate.(wywo�ywana rekurencyjnie dla wszystkich dzieci)

class Transform
{
public:
	Transform();//konstruktor bezargumentowy. element bez dieci i bez transformacji (np pivoty)
	Transform(glm::mat4 localTransform);//konstruktor kt�ry przyjmuje bazow� transformacj�

	void addChild(Transform* child);
	void recalculate(glm::mat4 parentTransform = glm::mat4(1.0f)); //oblicza graf sceny

	void reset();
	void simulate(); //oblicza rotacje planet na podstawie rotation speed (rekurencyjnie dla wszystkich dzieci)
	void move(glm::vec3 value);
	void rotate(glm::vec3 value);
	void scale(glm::vec3 value);

	glm::mat4 getWorldTransform();

	float rotationSpeed = 0.0f;

private:
	std::vector<Transform*> children;

	glm::mat4 localTransform; //transformacja wzgl�dem rodzica
	glm::mat4 worldTransform; //transformacja wzgl�dem �wiat (0,0,0)
};