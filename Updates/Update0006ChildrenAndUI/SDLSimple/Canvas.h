#pragma once
#include "Component.h"

class Canvas : public Component {
private:
	glm::vec3* m_cameraPos;

public:
	Canvas(GameObject* parent) :Component(parent) {};
	Canvas(GameObject* parent, glm::vec3* cameraPos) { m_cameraPos = cameraPos; };

	void update(float deltaTime);
};

