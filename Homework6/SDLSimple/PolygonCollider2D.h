#pragma once
#include "Collider2D.h"
#include "CircleCollider2D.h"

class PolygonCollider2D : public Collider2D {
private:
	std::vector<glm::vec3> m_vertices = {};
	std::vector<glm::vec3> m_verticesRelative = {};
public:
	PolygonCollider2D(GameObject* parent) :Collider2D(parent) { getComponent<Rigidbody2D>()->addCollider(this); };
	PolygonCollider2D(GameObject* parent, std::vector<glm::vec3> verticesRelative);

	void update(float deltaTime);

	void updateVertices();
	CollisionData isCollidingWith(Collider2D* otherCollider);
	CollisionData isCollidingWith(PolygonCollider2D* otherCollider);
	CollisionData isCollidingWith(CircleCollider2D* otherCollider);

	void projectVertices(std::vector<glm::vec3> vertices, glm::vec3 axis, float* min, float* max);
	void projectCircle(CircleCollider2D* otherCollider, glm::vec3 axis, float* min, float* max);
	void projectCircleMax(CircleCollider2D* otherCollider, glm::vec3 axis, float* min, float* max);
	float dotProduct(glm::vec3 a, glm::vec3 b) { return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z)); };
	glm::vec3 findClosestPolygonVertexToPoint(std::vector<glm::vec3> vertices, glm::vec3 point);
	glm::vec3 findPolygonCenter(std::vector<glm::vec3> vertices);

	// ————— GETTERS ————— //
	std::vector<glm::vec3> const getVertices() const { return m_vertices; };

	// ————— SETTERS ————— //
	void const setRelativeVertices(std::vector<glm::vec3> verticesRelative) { m_verticesRelative = verticesRelative; };
	void const addRelativeVertex(glm::vec3 vertex) { m_verticesRelative.push_back(vertex); };
	void const setColliderPosition(glm::vec3 newPosition) override { Collider2D::setColliderPosition(newPosition); updateVertices(); };
	void const setColliderPosition(XYZ xyz, float newVal) override { Collider2D::setColliderPosition(xyz, newVal); updateVertices(); };
	void const setColliderRotation(glm::vec3 newRotation) override { Collider2D::setColliderRotation(newRotation); updateVertices(); };
	void const setColliderRotation(XYZ xyz, float newVal) override { Collider2D::setColliderRotation(xyz, newVal); updateVertices(); };
	void const setColliderScale(glm::vec3 newScale) override { Collider2D::setColliderScale(newScale); updateVertices(); };
};

