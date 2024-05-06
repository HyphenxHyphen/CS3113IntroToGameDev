#pragma once
#include "Component.h"

class Canvas : public Component {
private:
	glm::vec3* m_origin = nullptr;

public:
	Canvas(GameObject* parent) :Component(parent) {};
	Canvas(GameObject* parent, glm::vec3* origin) { m_origin = origin; };

	void update(float deltaTime);
};

