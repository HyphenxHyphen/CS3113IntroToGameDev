#include "Rigidbody2D.h"
#include "Collider2D.h"
#include "PolygonCollider2D.h"

void Rigidbody2D::update(float deltaTime) {
	// Check Collisions
	checkCollisions();

	// Clean Collisions
	cleanCollisions();

	// Apply Physics
	if (m_rbType == DYNAMIC) {
		m_force += m_mass * m_acceleration; // F = ma
		m_velocity += (m_force / m_mass) * deltaTime; // v = at = f/m * t

		setPosition(X, getPosition().x + m_velocity.x * deltaTime);
		setPosition(Y, getPosition().y + m_velocity.y * deltaTime);

		m_force = { 0.0f, 0.0f, 0.0f };
	}
}

void Rigidbody2D::checkCollisions()
{
	for (auto gameObject : *getAllGameObjects()) {
		if (Rigidbody2D* rbOther = gameObject->getComponent<Rigidbody2D>()) {
			if (rbOther != this) {
				CollisionData collisionData = isCollidingWith(rbOther);
				if (collisionData.collided && (std::find(m_collisions.begin(), m_collisions.end(), rbOther) == m_collisions.end())) {
					//std::cout << getObjectID() << " at " << getPosition().x << ", " << getPosition().y << " entered collision with " << gameObject->getObjectID() << " at " << gameObject->getPosition().x << ", " << gameObject->getPosition().y << std::endl;
					//std::cout << collisionData.collided << " depth = " << collisionData.depth << " " << std::endl;
					m_collisions.push_back(rbOther);
				}
			}
		}
	}
}

void Rigidbody2D::cleanCollisions()
{
	for (int i = m_collisions.size() - 1; i >= 0; i--) {
		if (!(isCollidingWith(m_collisions[i]).collided)) {
			std::cout << getObjectID() << " exited collision with " << m_collisions[i]->getObjectID() << std::endl;
			m_collisions.erase(m_collisions.begin() + i);
		}
	}
}

CollisionData Rigidbody2D::isCollidingWith(Rigidbody2D* rbOther) {
	CollisionData collisionData;
	//std::cout << getObjectID() << " has " << m_colliders.size() << "colliders to " << rbOther->getObjectID() << " has " << rbOther->m_colliders.size() << std::endl;
	for (auto collider : m_colliders) {
		for (auto colliderOther : rbOther->m_colliders) {
			CollisionData collisionColliderData = collider->isCollidingWith(colliderOther);
			//std::cout << " collided? " << collisionColliderData.collided << std::endl;

			if (collisionColliderData.collided) {
				// Do collision expelling here
				std::cout << getObjectID() << " collided with " << rbOther->getObjectID() << " normal " << collisionColliderData.normal.x << ", " << collisionColliderData.normal.x << std::endl;

				
				if ((getRBType() == DYNAMIC) && (rbOther->getRBType() == DYNAMIC)) {
					setPosition(X, getPosition().x + (-collisionColliderData.normal.x * collisionColliderData.depth / 2.0f));
					setPosition(Y, getPosition().y + (-collisionColliderData.normal.y * collisionColliderData.depth / 2.0f));
					rbOther->setPosition(X, rbOther->getPosition().x + (collisionColliderData.normal.x * collisionColliderData.depth / 2.0f));
					rbOther->setPosition(Y, rbOther->getPosition().y + (collisionColliderData.normal.y * collisionColliderData.depth / 2.0f));

					//m_velocity -= (collisionColliderData.normal.x * collisionColliderData.depth / 2.0f);
					//m_velocity = { 0.0f, 0.0f, 0.0f };
				}
				else if ((getRBType() == DYNAMIC) && (rbOther->getRBType() == KINEMATIC)) {
					setPosition(X, getPosition().x + (-collisionColliderData.normal.x * collisionColliderData.depth));
					setPosition(Y, getPosition().y + (-collisionColliderData.normal.y * collisionColliderData.depth));
					//otherRB->setPosition(X, otherRB->getPosition().x + (collisionColliderData.normal.x * collisionColliderData.depth / 2.0f));
					//otherRB->setPosition(Y, otherRB->getPosition().y + (collisionColliderData.normal.y * collisionColliderData.depth / 2.0f));
				}
				else if ((getRBType() == DYNAMIC) && (rbOther->getRBType() == STATIC)) {
					setPosition(X, getPosition().x + (-collisionColliderData.normal.x * collisionColliderData.depth));
					setPosition(Y, getPosition().y + (-collisionColliderData.normal.y * collisionColliderData.depth));
					//otherRB->setPosition(X, otherRB->getPosition().x + (collisionData.normal.x * collisionData.depth / 2.0f));
					//otherRB->setPosition(Y, otherRB->getPosition().y + (collisionData.normal.y * collisionData.depth / 2.0f));
				}
				else {
					collisionData = collisionColliderData;
				}
			}
		}
	}
	
	return collisionData;
};

void const Rigidbody2D::removeCollider(Collider2D* collider) {
	auto it = std::find(m_colliders.begin(), m_colliders.end(), collider); 
	if (it != m_colliders.end()) {
		m_colliders.erase(it);
	}
}