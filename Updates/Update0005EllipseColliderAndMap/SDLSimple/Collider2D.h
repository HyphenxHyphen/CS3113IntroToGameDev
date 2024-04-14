#pragma once
#include "Component.h"
#include "Rigidbody2D.h"

class Collider2D : public Component
{
protected:
	glm::vec3 m_colliderPosition = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_colliderRotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_colliderScale = { 1.0f, 1.0f, 1.0f };

public:
	Collider2D() {};
	Collider2D(GameObject* parent) :Component(parent) {};
	virtual ~Collider2D() { getComponent<Rigidbody2D>()->removeCollider(this); };
	virtual CollisionData isCollidingWith(Collider2D* otherCollider) = 0;

	// ————— GETTERS ————— //
	glm::vec3 const getColliderPosition() const { return m_colliderPosition; };
	glm::vec3 const getColliderRotation() const { return m_colliderRotation; };
	glm::vec3 const getColliderScale() const { return m_colliderScale; };

	// ————— SETTERS ————— //
	void const setColliderPosition(glm::vec3 newPosition) { m_colliderPosition = newPosition; };
	void const setColliderPosition(XYZ xyz, float newVal) { m_colliderPosition[xyz] = newVal; };
	void const setColliderRotation(glm::vec3 newRotation) { m_colliderRotation = newRotation; };
	void const setColliderRotation(XYZ xyz, float newVal) { m_colliderRotation[xyz] = newVal; };
	void const setColliderScale(glm::vec3 newScale) { m_colliderScale = newScale; };
};

