#include "Rigidbody2D.h"
#include "Collider2D.h"
#include "PolygonCollider2D.h"

void Rigidbody2D::update(float deltaTime) {
	// Check Collisions
	checkCollisions();

	// Do collision expelling here


	// Clean Collisions
	cleanCollisions();


	// Apply Physics
	m_force += m_mass * m_acceleration; // F = ma
	m_velocity += (m_force / m_mass) * deltaTime; // v = at = f/m * t
	
	setPosition(X, getPosition().x + m_velocity.x * deltaTime);
	setPosition(Y, getPosition().y + m_velocity.y * deltaTime);

	m_force = { 0.0f, 0.0f, 0.0f };
}

void Rigidbody2D::checkCollisions()
{
	for (auto gameObject : *getAllGameObjects()) {
		if (Rigidbody2D* rbOther = gameObject->getComponent<Rigidbody2D>()) {
			if (rbOther != this) {
				if (isCollidingWith(rbOther) && (std::find(m_collisions.begin(), m_collisions.end(), rbOther) == m_collisions.end())) {
					std::cout << getObjectID() << " entered collision with " << gameObject->getObjectID() << std::endl;
					//std::cout << getObjectID() << " is at (" << getPosition().x << ", " << getPosition().y << ", " << getPosition().z << ") " << std::endl;

					m_collisions.push_back(rbOther);
				}
			}
		}
	}
}

void Rigidbody2D::cleanCollisions()
{
	for (int i = m_collisions.size() - 1; i >= 0; i--) {
		if (!isCollidingWith(m_collisions[i])) {
			std::cout << getObjectID() << " entered collision with " << m_collisions[i]->getObjectID() << std::endl;
			m_collisions.erase(m_collisions.begin() + i);
		}
	}
}

bool Rigidbody2D::isCollidingWith(Rigidbody2D* otherRB) {
	for (auto collider : m_colliders) {
		for (auto colliderOther : otherRB->m_colliders) {
			CollisionData collisionData = collider->isCollidingWith(colliderOther);

			if (collisionData.collided) {
				setPosition(X, getPosition().x + (-collisionData.normal.x * collisionData.depth / 2.0f));
				setPosition(Y, getPosition().y + (-collisionData.normal.y * collisionData.depth / 2.0f));
				otherRB->setPosition(X, otherRB->getPosition().x + (collisionData.normal.x * collisionData.depth / 2.0f));
				otherRB->setPosition(Y, otherRB->getPosition().y + (collisionData.normal.y * collisionData.depth / 2.0f));
				return true;
			}
		}
	}
	return false;
};

void const Rigidbody2D::removeCollider(Collider2D* collider) {
	auto it = std::find(m_colliders.begin(), m_colliders.end(), collider); 
	if (it != m_colliders.end()) {
		m_colliders.erase(it);
	}
}