#pragma once
#include "Collider2D.h"

class CircleCollider2D : public Collider2D
{
private:
	glm::vec3 m_vertex = { 1.0f, 1.0f, 0.0f };
	glm::vec3 m_vertexRelative = { 1.0f, 1.0f, 0.0f };
public:
	CircleCollider2D(GameObject* parent) :Collider2D(parent) { getComponent<Rigidbody2D>()->addCollider(this); };
	CircleCollider2D(GameObject* parent, glm::vec3 vertex);

	void update(float deltaTime);

	CollisionData isCollidingWith(Collider2D* otherCollider);
	CollisionData isCollidingWith(CircleCollider2D* otherCollider);

	//void projectVertices(std::vector<glm::vec3> vertices, glm::vec3 axis, float* min, float* max);
	//float dotProduct(glm::vec3 a, glm::vec3 b) { return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z)); };

	// ————— GETTERS ————— //
	glm::vec3 const getVertex() const { return m_vertex; };

	// ————— SETTERS ————— //
	//void const setRelativeVertices(std::vector<glm::vec3> verticesRelative) { m_verticesRelative = verticesRelative; };
	//void const addRelativeVertex(glm::vec3 vertex) { m_verticesRelative.push_back(vertex); };
};

