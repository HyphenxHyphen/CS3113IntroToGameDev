#pragma once
#include "Component.h"
#include "EntityController.h"
#include "Rigidbody2D.h"
#include "SpriteRenderer.h"

class GoblinAI : public Component
{
private:
	EntityController* ec;
	Rigidbody2D* rb;
	SpriteRenderer* sr;
	glm::vec3 m_wanderTarget = { 0.0f, 0.0f, 0.0f };

public:
	GoblinAI(GameObject* parent) :Component(parent) { ec = getComponent<EntityController>(); rb = getComponent<Rigidbody2D>(); sr = getComponent<SpriteRenderer>(); };
	void update(float deltaTime);
};

