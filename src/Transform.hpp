#pragma once
#include <vector>
#include <glm/glm.hpp>

//implementacja grafu sceny tak naprawde
//ka¿dy transform jest przedstawieniem translacji rotacji i skali w przestrzeni 3d
//transformy u³o¿one s¹ w hierarchiê; ka¿dy transform posiada wskaŸniki na swoje dzieci.
//Jeœli poruszymy rodzicem to porusza siê te¿ jego dziecko. W ten sam sposób ofc.
//Funkcjonalnoœæ t¹ funkcjonalnoœæ zapewnia funkcja recalculate.(wywo³ywana rekurencyjnie dla wszystkich dzieci)

class Transform
{
public:
	Transform();//konstruktor bezargumentowy. element bez dieci i bez transformacji (np pivoty)
	Transform(glm::mat4 localTransform);//konstruktor który przyjmuje bazow¹ transformacjê

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

	glm::mat4 localTransform; //transformacja wzglêdem rodzica
	glm::mat4 worldTransform; //transformacja wzglêdem œwiat (0,0,0)
};