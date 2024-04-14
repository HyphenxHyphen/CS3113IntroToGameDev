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
	if (PolygonCollider2D * otherPolygonCollider = dynamic_cast<PolygonCollider2D*>(otherCollider))
	{
		return isCollidingWith(otherPolygonCollider);
	}
	else if (CircleCollider2D * otherCircleCollider = dynamic_cast<CircleCollider2D*>(otherCollider))
	{
		// BUGGY
		return isCollidingWith(otherCircleCollider);
	}
	return CollisionData();
}

CollisionData PolygonCollider2D::isCollidingWith(PolygonCollider2D* otherCollider) {
	CollisionData collisionData;

	for (unsigned int i = 0; i < m_vertices.size(); i++) {
		glm::vec3 vA = m_vertices[i];
		glm::vec3 vB = m_vertices[(i + 1) % m_vertices.size()];

		glm::vec3 edge = vB - vA;
		glm::vec3 axis = { -edge.y, edge.x, edge.z };
		axis = glm::normalize(axis);

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
		axis = glm::normalize(axis);

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

	//collisionData.depth /= glm::length(collisionData.normal);
	//collisionData.normal = glm::normalize(collisionData.normal);
	collisionData.collided = true;
	/*
	std::cout << getObjectID() << " Polygon Vertices Count: " << m_vertices.size() << std::endl;
	for (auto vertex : m_vertices) {
		std::cout << "Vertex is at (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ") " << std::endl;
	}
	*/

	if (dotProduct(findPolygonCenter(otherCollider->m_vertices) - findPolygonCenter(m_vertices), collisionData.normal) < 0.0f) {
		collisionData.normal *= -1.0f;
	}

	return collisionData;
}

CollisionData PolygonCollider2D::isCollidingWith(CircleCollider2D* otherCollider) {
	CollisionData collisionData;
	glm::vec3 axis = { 0.0f, 0.0f, 0.0f };

	float minA;
	float maxA;
	float minB;
	float maxB;
	float axisDepth = 0;

	// Unused loop to test for bounds of ellipse
	/*
	for (unsigned int i = 0; i < m_vertices.size(); i++) {
		glm::vec3 vA = m_vertices[i];
		glm::vec3 vB = m_vertices[(i + 1) % m_vertices.size()];

		glm::vec3 edge = vB - vA;
		axis = { -edge.y, edge.x, edge.z };
		axis = glm::normalize(axis);

		projectVertices(m_vertices, axis, &minA, &maxA);
		projectCircleMax(otherCollider, axis, &minB, &maxB);

		if ((minA >= maxB) || (minB >= maxA)) {
			return collisionData;
		}

		axisDepth = fabs(std::fmin(maxB - minA, maxA - minB));
		if (axisDepth < collisionData.depth) {
			collisionData.depth = axisDepth;
			collisionData.normal = axis;
		}
	}

	std::cout << "Pass Bounding "
		<< std::endl;
	*/
	
	axis = (otherCollider->getPosition() + otherCollider->getColliderPosition()) - findClosestPolygonVertexToPoint(m_vertices, (otherCollider->getPosition() + otherCollider->getColliderPosition()));
	axis = glm::normalize(axis);

	//axis = { -axis.y, axis.x, axis.z };

	//std::cout << "Axis {"
	//	<< axis.x << ", " << axis.y << "} "
	//	<< std::endl;

	projectVertices(m_vertices, axis, &minA, &maxA);
	projectCircle(otherCollider, axis, &minB, &maxB);

	if ((minA >= maxB) || (minB >= maxA)) {
		return collisionData;
	}
	

	axisDepth = fabs(std::fmin(maxB - minA, maxA - minB));
	if (axisDepth < collisionData.depth) {
		collisionData.depth = axisDepth;
		collisionData.normal = axis;
	}

	//collisionData.depth /= glm::length(collisionData.normal);
	//collisionData.normal = glm::normalize(collisionData.normal);
	collisionData.collided = true;

	if (dotProduct(((otherCollider->getPosition() + otherCollider->getColliderPosition()) - findPolygonCenter(m_vertices)), collisionData.normal) < 0.0f) {
		collisionData.normal *= -1.0f;
	}

	//std::cout << " Collided " << std::endl;

	return collisionData;
}

void PolygonCollider2D::projectVertices(std::vector<glm::vec3> vertices, glm::vec3 axis, float* min, float* max) {
	*min = dotProduct(vertices[0], axis);
	*max = dotProduct(vertices[0], axis);
	for (int i = 0; i < vertices.size(); i++) {
		float projection = dotProduct(vertices[i], axis);
		if (projection < *min) { *min = projection; }
		if (projection > *max) { *max = projection; }
	}

	//std::cout << getObjectID() << " Polygon Point " << *min << " Max: " << *max << std::endl;
}

void PolygonCollider2D::projectCircle(CircleCollider2D* otherCollider, glm::vec3 axis, float* min, float* max) {
	float theta = glm::radians(otherCollider->getColliderRotation().z + otherCollider->getRotation().z);
	float aPhi = atan2((otherCollider->getVertex().x * (axis.y / axis.x - tan(theta))), (otherCollider->getVertex().y * (1 + (axis.y / axis.x) * tan(theta))));
	//std::cout << (atan2(0,0)) << " aPhi: " << aPhi << " deg " << glm::degrees(aPhi) << std::endl;

	glm::vec3 aPoint(
		(otherCollider->getColliderPosition() + otherCollider->getPosition()).x + (otherCollider->getVertex().x * cos(aPhi) * cos(theta) - otherCollider->getVertex().y * sin(aPhi) * sin(theta)),
		(otherCollider->getColliderPosition() + otherCollider->getPosition()).y + (otherCollider->getVertex().x * cos(aPhi) * sin(theta) + otherCollider->getVertex().y * sin(aPhi) * cos(theta)),
		0.0f);

	float bPhi = aPhi + glm::radians(180.0f);
	glm::vec3 bPoint(
		(otherCollider->getColliderPosition() + otherCollider->getPosition()).x + (otherCollider->getVertex().x * cos(bPhi) * cos(theta) - otherCollider->getVertex().y * sin(bPhi) * sin(theta)),
		(otherCollider->getColliderPosition() + otherCollider->getPosition()).y + (otherCollider->getVertex().x * cos(bPhi) * sin(theta) + otherCollider->getVertex().y * sin(bPhi) * cos(theta)),
		0.0f);

	*min = dotProduct(aPoint, axis);
	*max = dotProduct(bPoint, axis);
	
	if (*min > *max) {
		float t = *min;
		*min = *max;
		*max = t;
	}

	//std::cout << "Min: " << *min << " Max: " << *max << " Circle At {" 
	//	<< (otherCollider->getColliderPosition() + otherCollider->getPosition()).x << ", " << (otherCollider->getColliderPosition() + otherCollider->getPosition()).y << "} Points: {"
	//	<< (aPoint).x << ", " << (aPoint).y << "} {"
	//	<< (bPoint).x << ", " << (bPoint).y << "} "
	//	<< std::endl;

}

void PolygonCollider2D::projectCircleMax(CircleCollider2D* otherCollider, glm::vec3 axis, float* min, float* max) {
	*min = std::numeric_limits<float>::max();
	*max = std::numeric_limits<float>::min();

	glm::vec3 aDirection = axis;
	float aTheta = atan2(aDirection.y, aDirection.x);
	aTheta += glm::radians(otherCollider->getColliderRotation().z + otherCollider->getRotation().z);

	float axPhi = atan(-otherCollider->getVertex().y * tan(aTheta) / otherCollider->getVertex().x);
	float ayPhi = atan(otherCollider->getVertex().y * (1 / tan(aTheta)) / otherCollider->getVertex().x);

	glm::vec3 aPoint(
		(otherCollider->getColliderPosition() + otherCollider->getPosition()).x + otherCollider->getVertex().x * cos(aTheta) * cos(axPhi) - otherCollider->getVertex().y * sin(aTheta) * sin(axPhi),
		(otherCollider->getColliderPosition() + otherCollider->getPosition()).y + otherCollider->getVertex().x * cos(aTheta) * sin(ayPhi) + otherCollider->getVertex().y * sin(aTheta) * cos(ayPhi),
		0.0f);

	float bTheta = aTheta;
	float bxPhi = axPhi + glm::radians(180.0f);
	float byPhi = ayPhi + glm::radians(180.0f);

	glm::vec3 bPoint(
		(otherCollider->getColliderPosition() + otherCollider->getPosition()).x + otherCollider->getVertex().x * cos(bTheta) * cos(bxPhi) - otherCollider->getVertex().y * sin(bTheta) * sin(bxPhi),
		(otherCollider->getColliderPosition() + otherCollider->getPosition()).y + otherCollider->getVertex().x * cos(bTheta) * sin(byPhi) + otherCollider->getVertex().y * sin(bTheta) * cos(byPhi),
		0.0f);

	*min = dotProduct(aPoint, axis);
	*max = dotProduct(bPoint, axis);

	if (*min > * max) {
		float t = *min;
		*min = *max;
		*max = t;
	}

	//std::cout << otherCollider->getObjectID() << " MAXCircle Min: " << *min << " Max: " << *max << std::endl;
}

glm::vec3 PolygonCollider2D::findClosestPolygonVertexToPoint(std::vector<glm::vec3> vertices, glm::vec3 point) {
	int result = 0;
	float minDistance = std::numeric_limits<float>::max();

	for (int i = 0; i < vertices.size(); i++) {
		glm::vec3 vertex = vertices[i];
		float distance = sqrt(pow(vertex.x - point.x, 2) + pow(vertex.y - point.y, 2));
		if (distance < minDistance) {
			minDistance = distance;
			result = i;
		}
	}

	//std::cout << "Closest Polygon Point " << result << " {"
	//	<< vertices[result].x << ", " << vertices[result].y << "} "
	//	<< std::endl;

	return vertices[result];
}

glm::vec3 PolygonCollider2D::findPolygonCenter(std::vector<glm::vec3> vertices) {
	glm::vec3 center({ 0.0f });
	for (auto& vertex : vertices) {
		center += vertex;
	}
	center /= vertices.size();
	return center;
}