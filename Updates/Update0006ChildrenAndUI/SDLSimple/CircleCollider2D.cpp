#include "CircleCollider2D.h"

CircleCollider2D::CircleCollider2D(GameObject* parent, glm::vec3 vertex) :Collider2D(parent) {
	getComponent<Rigidbody2D>()->addCollider(this);
	m_vertexRelative = vertex;
	m_vertex = m_vertexRelative * m_colliderScale * getScale();
};

void CircleCollider2D::update(float deltaTime) {
	m_vertex = m_vertexRelative * m_colliderScale * getScale();
}

CollisionData CircleCollider2D::isCollidingWith(Collider2D* otherCollider) {
	if (CircleCollider2D* otherCircleCollider = dynamic_cast<CircleCollider2D*>(otherCollider))
	{
		return isCollidingWith(otherCircleCollider);
	}
	return CollisionData();
}

CollisionData CircleCollider2D::isCollidingWith(CircleCollider2D* otherCollider) {
	CollisionData collisionData;
	
	/*
	glm::vec3 aDirection = (otherCollider->getColliderPosition() + otherCollider->getPosition()) - (getColliderPosition() + getPosition());
	float aTheta = atan2(aDirection.y, aDirection.x);
	aTheta -= glm::radians(getColliderRotation().z + getRotation().z);
	glm::vec3 aPoint(m_vertex.x * cos(aTheta), m_vertex.y * sin(aTheta), 0.0f);
	float aRadius = sqrt(pow(aPoint.x, 2) + pow(aPoint.y, 2));

	glm::vec3 bDirection = (getColliderPosition() + getPosition()) - (otherCollider->getColliderPosition() + otherCollider->getPosition());
	float bTheta = atan2(bDirection.y, bDirection.x);
	bTheta -= glm::radians(otherCollider->getColliderRotation().z + otherCollider->getRotation().z);
	glm::vec3 bPoint(otherCollider->m_vertex.x * cos(bTheta), otherCollider->m_vertex.y * sin(bTheta), 0.0f);
	float bRadius = sqrt(pow(bPoint.x, 2) + pow(bPoint.y, 2));	
	*/

	glm::vec3 axis = (otherCollider->getColliderPosition() + otherCollider->getPosition()) - (getColliderPosition() + getPosition());

	float aTheta = glm::radians(getColliderRotation().z + getRotation().z);
	float aPhi = aTheta - atan2(axis.y, axis.x);
	glm::vec3 aPoint(
		(getColliderPosition() + getPosition()).x + (getVertex().x * cos(aPhi) * cos(aTheta) - getVertex().y * sin(aPhi) * sin(aTheta)),
		(getColliderPosition() + getPosition()).y + (getVertex().x * cos(aPhi) * sin(aTheta) + getVertex().y * sin(aPhi) * cos(aTheta)),
		0.0f);
	float aRadius = sqrt(pow(aPoint.x - (getColliderPosition() + getPosition()).x, 2) + pow(aPoint.y - (getColliderPosition() + getPosition()).y, 2));


	float bTheta = glm::radians(otherCollider->getColliderRotation().z + otherCollider->getRotation().z);
	float bPhi = bTheta - atan2(axis.y, axis.x);
	glm::vec3 bPoint(
		(otherCollider->getColliderPosition() + otherCollider->getPosition()).x + (otherCollider->getVertex().x * cos(bPhi) * cos(bTheta) - otherCollider->getVertex().y * sin(bPhi) * sin(bTheta)),
		(otherCollider->getColliderPosition() + otherCollider->getPosition()).y + (otherCollider->getVertex().x * cos(bPhi) * sin(bTheta) + otherCollider->getVertex().y * sin(bPhi) * cos(bTheta)),
		0.0f);
	float bRadius = sqrt(pow(bPoint.x - (otherCollider->getColliderPosition() + otherCollider->getPosition()).x, 2) + pow(bPoint.y - (otherCollider->getColliderPosition() + otherCollider->getPosition()).y, 2));

	float abDistance = sqrt(pow((otherCollider->getColliderPosition().x + otherCollider->getPosition().x) - (getColliderPosition().x + getPosition().x), 2) + pow((otherCollider->getColliderPosition().y + otherCollider->getPosition().y) - (getColliderPosition().y + getPosition().y), 2));
	if (abDistance < aRadius + bRadius) {
		collisionData.collided = true;
		collisionData.normal = glm::normalize(axis);
		collisionData.depth = (aRadius + bRadius) - abDistance;
		//std::cout << "Ax: " << m_vertex.x << ", Bx: " << otherCollider->m_vertex.x << std::endl;
		//std::cout << "ARad: " << aRadius << ", BRad: " << bRadius << ", abDistance: " << abDistance << std::endl;
	}

	return collisionData;
}