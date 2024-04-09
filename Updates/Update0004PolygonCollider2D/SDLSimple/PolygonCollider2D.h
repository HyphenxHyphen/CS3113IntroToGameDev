#pragma once
#include "Collider2D.h"
#include "Rigidbody2D.h"

class PolygonCollider2D : public Collider2D {
private:
	std::vector<glm::vec3> m_vertices = {};
	std::vector<glm::vec3> m_verticesRelative = {};
public:
	PolygonCollider2D(GameObject* parent) :Collider2D(parent) { getComponent<Rigidbody2D>()->addCollider(this); };
	PolygonCollider2D(GameObject* parent, std::vector<glm::vec3> verticesRelative);
	virtual ~PolygonCollider2D() { getComponent<Rigidbody2D>()->removeCollider(this); };

	void update(float deltaTime);

	CollisionData isCollidingWith(Collider2D* otherCollider);
	CollisionData isCollidingWith(PolygonCollider2D* otherCollider);

	void projectVertices(std::vector<glm::vec3> vertices, glm::vec3 axis, float* min, float* max);
	float dotProduct(glm::vec3 a, glm::vec3 b) { return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z)); };

	// ————— GETTERS ————— //
	std::vector<glm::vec3> const getVertices() const { return m_vertices; };

	// ————— SETTERS ————— //
	void const setRelativeVertices(std::vector<glm::vec3> verticesRelative) { m_verticesRelative = verticesRelative; };
	void const addRelativeVertex(glm::vec3 vertex) { m_verticesRelative.push_back(vertex); };
};

