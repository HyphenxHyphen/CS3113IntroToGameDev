#include "PolygonCollider2D.h"

PolygonCollider2D::PolygonCollider2D(GameObject* parent, std::vector<glm::vec3> verticesRelative) :Collider2D(parent) {
	getComponent<Rigidbody2D>()->addCollider(this);

	m_verticesRelative = verticesRelative; 
	for (unsigned int i = 0; i < m_verticesRelative.size(); i++) {
		m_vertices.push_back({ 0.0f, 0.0f, 0.0f });

		// Translate based on collider position
		m_vertices[i] = (m_verticesRelative[i] + m_colliderPosition);

		// Scale based on collider scale and object scale
		m_vertices[i] *= m_colliderScale * getScale();

		// Translate onto object position
		m_vertices[i] += getPosition();

		// Rotate around object based on collider rotation and object rotation
		float theta = glm::radians(m_colliderRotation.z + getRotation().z);
		m_vertices[i].x = (cos(theta) * (m_vertices[i].x - getPosition().x)) - (sin(theta) * (m_vertices[i].y - getPosition().y)) + getPosition().x;
		m_vertices[i].y = (sin(theta) * (m_vertices[i].x - getPosition().x)) + (cos(theta) * (m_vertices[i].y - getPosition().y)) + getPosition().y;
	}
};

void PolygonCollider2D::update(float deltaTime) {
	for (unsigned int i = 0; i < m_verticesRelative.size(); i++) {
		// Translate based on collider position
		m_vertices[i] = (m_verticesRelative[i] + m_colliderPosition);

		// Scale based on collider scale and object scale
		m_vertices[i] *= m_colliderScale * getScale();

		// Translate onto object position
		m_vertices[i] += getPosition();

		// Rotate around object based on collider rotation and object rotation
		float theta = glm::radians(m_colliderRotation.z + getRotation().z);

		float newX = (cos(theta) * (m_vertices[i].x - getPosition().x)) - (sin(theta) * (m_vertices[i].y - getPosition().y)) + getPosition().x;
		float newY = (sin(theta) * (m_vertices[i].x - getPosition().x)) + (cos(theta) * (m_vertices[i].y - getPosition().y)) + getPosition().y;
		m_vertices[i].x = newX;
		m_vertices[i].y = newY;
	}
}

CollisionData PolygonCollider2D::isCollidingWith(Collider2D* otherCollider) {
	if (PolygonCollider2D * otherBoxCollider = dynamic_cast<PolygonCollider2D*>(otherCollider))
	{
		return isCollidingWith(otherBoxCollider);
	}
}

CollisionData PolygonCollider2D::isCollidingWith(PolygonCollider2D* otherCollider) {
	CollisionData collisionData;

	for (unsigned int i = 0; i < m_vertices.size(); i++) {
		glm::vec3 vA = m_vertices[i];
		glm::vec3 vB = m_vertices[(i + 1) % m_vertices.size()];

		glm::vec3 edge = vB - vA;
		glm::vec3 axis = { -edge.y, edge.x, edge.z };

		float minA;
		float maxA;
		projectVertices(m_vertices, axis, &minA, &maxA);
		float minB;
		float maxB;
		projectVertices(otherCollider->m_vertices, axis, &minB, &maxB);

		if ((minA >= maxB) || (minB >= maxA)) {
			return collisionData;
		}

		float axisDepth = std::fmin(maxB - minA, maxA - minB);
		if (axisDepth < collisionData.depth) {
			collisionData.depth = axisDepth;
			collisionData.normal = axis;
		}
	}

	for (unsigned int i = 0; i < otherCollider->m_vertices.size(); i++) {
		glm::vec3 vA = otherCollider->m_vertices[i];
		glm::vec3 vB = otherCollider->m_vertices[(i + 1) % otherCollider->m_vertices.size()];

		glm::vec3 edge = vB - vA;
		glm::vec3 axis = { -edge.y, edge.x, edge.z };

		float minA;
		float maxA;
		projectVertices(m_vertices, axis, &minA, &maxA);
		float minB;
		float maxB;
		projectVertices(otherCollider->m_vertices, axis, &minB, &maxB);

		if ((minA >= maxB) || (minB >= maxA)) {
			return collisionData;
		}

		float axisDepth = std::fmin(maxB - minA, maxA - minB);
		if (axisDepth < collisionData.depth) {
			collisionData.depth = axisDepth;
			collisionData.normal = axis;
		}
	}

	collisionData.depth /= glm::length(collisionData.normal);
	collisionData.normal = glm::normalize(collisionData.normal);
	collisionData.collided = true;
	/*
	std::cout << getObjectID() << " Polygon Vertices Count: " << m_vertices.size() << std::endl;
	for (auto vertex : m_vertices) {
		std::cout << "Vertex is at (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ") " << std::endl;
	}
	*/

	if (dotProduct((otherCollider->getPosition() - getPosition()), collisionData.normal) < 0.0f) {
		collisionData.normal *= -1.0f;
	}

	return collisionData;
}

void PolygonCollider2D::projectVertices(std::vector<glm::vec3> vertices, glm::vec3 axis, float* min, float* max) {
	*min = std::numeric_limits<float>::max();
	*max = std::numeric_limits<float>::min();
	for (int i = 0; i < vertices.size(); i++) {
		float projection = dotProduct(vertices[i], axis);
		if (projection < *min) { *min = projection; }
		if (projection > * max) { *max = projection; }
	}

}