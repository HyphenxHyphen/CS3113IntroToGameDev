#include "GoblinAI.h"

void GoblinAI::update(float deltaTime) {

	if ((getComponent<EntityController>()->getTarget() != nullptr) && (getComponent<EntityController>()->getTarget()->getParent()->isAlive())) {
		glm::vec3 targetPos = getComponent<EntityController>()->getTarget()->getPosition();
		if ((getComponent<EntityController>()->getTarget()->getActionState() == JUMPING) || (getComponent<EntityController>()->getTarget()->getActionState() == FALLING)) {
			targetPos = getComponent<EntityController>()->getTarget()->getFallTarget();
		}

		if (ec->getActionState() == IDLE) {
			if ((rand() % 1000) < 5) {
				ec->setActionState(WANDER);
				sr->setAnimActive(1);
				m_wanderTarget = {
				getPosition().x + (rand() % 3) * (((rand() % 100) <= 50) ? 1.0f : -1.0f),
				getPosition().y + (rand() % 3) * (((rand() % 100) <= 50) ? 1.0f : -1.0f),
				getPosition().z };
			}
		}

		if ((ec->getActionState() == IDLE) && ((getComponent<EntityController>()->getTarget()->getActionState() != ATTACKING) || (glm::distance(getPosition(), getComponent<EntityController>()->getTarget()->getPosition()) <= 0.5f))) {
			if (glm::distance(getPosition(), getComponent<EntityController>()->getTarget()->getPosition()) <= 3.0f) {
				if (((getComponent<EntityController>()->getTarget()->getActionState() != JUMPING) && (getComponent<EntityController>()->getTarget()->getActionState() != FALLING)) || (glm::distance(getPosition(), targetPos) > 0.5f)) {
					ec->setActionState(MOVING);
					sr->setAnimActive(1);
				}
			}

			if (((getComponent<EntityController>()->getTarget()->getActionState() == JUMPING) || (getComponent<EntityController>()->getTarget()->getActionState() == FALLING)) && (getComponent<EntityController>()->getTarget()->getComponent<Rigidbody2D>()->getVelocity().y < 0)) {
				//std::cout << "target is about to fall!" << std::endl;
				if (glm::distance(getPosition(), getComponent<EntityController>()->getTarget()->getFallTarget()) <= 0.5f) {
					//std::cout << "I am near target fallTarget!" << std::endl;
					if (glm::distance(getComponent<EntityController>()->getTarget()->getFallTarget(), getComponent<EntityController>()->getTarget()->getPosition()) <= 0.9f) {
						//std::cout << "Target is near fallTarget!" << std::endl;
						// Fall Prediction
						ec->setActionState(ATTACKING);
						if (fabs(getPosition().x - getComponent<EntityController>()->getTarget()->getPosition().x) < fabs(getPosition().y - getComponent<EntityController>()->getTarget()->getPosition().y)) {
							if (getPosition().x < getComponent<EntityController>()->getTarget()->getPosition().x) {
								ec->setFaceDirection(RIGHT);
								setRotation({ getRotation().x, 0.0f, getRotation().z });
								sr->setAnimActive(2);
							}
							else {
								ec->setFaceDirection(LEFT);
								setRotation({ getRotation().x, 180.0f, getRotation().z });
								sr->setAnimActive(2);
							}
						}
						else {
							if (getPosition().y > getComponent<EntityController>()->getTarget()->getPosition().y) {
								ec->setFaceDirection(DOWN);
								sr->setAnimActive(3);
							}
							else {
								ec->setFaceDirection(UP);
								sr->setAnimActive(4);
							}
						}
					}
				}
			}
		}

		if (ec->getActionState() == MOVING) {
			//std::cout << "------ move called ------" << std::endl;
			rb->setVelocity({
				((fabs(getPosition().x - targetPos.x) <= 0.1f) ? 0.0f : (getPosition().x > targetPos.x) ? -1.0f : 1.0f) * (((getComponent<EntityController>()->getTarget()->getActionState() == ATTACKING) && (glm::distance(getPosition(), getComponent<EntityController>()->getTarget()->getPosition()) <= 0.5f)) ? -1.0f : 1.0f),
				((fabs(getPosition().y - targetPos.y) <= 0.1f) ? 0.0f : (getPosition().y > targetPos.y) ? -1.0f : 1.0f) * (((getComponent<EntityController>()->getTarget()->getActionState() == ATTACKING) && (glm::distance(getPosition(), getComponent<EntityController>()->getTarget()->getPosition()) <= 0.5f)) ? -1.0f : 1.0f),
				0.0f
			});
			if (glm::length(rb->getVelocity()) > 1.0f)
			{
				rb->setVelocity(glm::normalize(rb->getVelocity()));
			}
			rb->setVelocity(rb->getVelocity() * ec->getMoveSpeed());
			if (rb->getVelocity().x < 0) {
				setRotation({ getRotation().x, 180.0f, getRotation().z });
			}
			else {
				setRotation({ getRotation().x, 0.0f, getRotation().z });
			}
			if (glm::distance(getPosition(), targetPos) > 4.0f) {
				rb->setVelocity({ 0.0f, 0.0f, 0.0f });
				ec->setActionState(IDLE);
				sr->setAnimActive(0);
			}
			if ((glm::distance(getPosition(), targetPos) <= 0.25f)) {
				rb->setVelocity({ 0.0f, 0.0f, 0.0f });
				ec->setActionState(IDLE);
				sr->setAnimActive(0);
			}

			if ((glm::distance(getPosition(), getComponent<EntityController>()->getTarget()->getPosition()) <= 0.5f) ||
				(glm::distance(getPosition(), (getComponent<EntityController>()->getTarget()->getPosition() + (getComponent<EntityController>()->getTarget()->getComponent<Rigidbody2D>()->getVelocity() * deltaTime * 22.0f))) <= 0.5f)) { // Prefiring Prediction for 22 frames
				rb->setVelocity({ 0.0f, 0.0f, 0.0f });
				ec->setActionState(ATTACKING);
				if (fabs(getPosition().x - getComponent<EntityController>()->getTarget()->getPosition().x) > fabs(getPosition().y - getComponent<EntityController>()->getTarget()->getPosition().y)) {
					if (getPosition().x < getComponent<EntityController>()->getTarget()->getPosition().x) {
						ec->setFaceDirection(RIGHT);
						setRotation({ getRotation().x, 0.0f, getRotation().z });
						sr->setAnimActive(2);
					}
					else {
						ec->setFaceDirection(LEFT);
						setRotation({ getRotation().x, 180.0f, getRotation().z });
						sr->setAnimActive(2);
					}
				}
				else {
					if (getPosition().y > getComponent<EntityController>()->getTarget()->getPosition().y) {
						ec->setFaceDirection(DOWN);
						sr->setAnimActive(3);
					}
					else {
						ec->setFaceDirection(UP);
						sr->setAnimActive(4);
					}
				}
			}

			if (getComponent<EntityController>()->getTarget()->getActionState() == ATTACKING) {
				rb->setVelocity({ 0.0f, 0.0f, 0.0f });
				ec->setActionState(IDLE);
				sr->setAnimActive(0);
			}
		}

		if (ec->getActionState() == ATTACKING) {
			if (sr->getAnimIndex() == 4) {
				ec->setActionState(STUNNED);
				// Knockback
				//std::cout << "FACING " << m_faceDirection << std::endl;

				glm::vec3 knockbackForce = {
					(ec->getFaceDirection() == RIGHT) ? 1.0f : (ec->getFaceDirection() == LEFT) ? -1.0f : 0.0f,
					(ec->getFaceDirection() == UP) ? 1.0f : (ec->getFaceDirection() == DOWN) ? -1.0f : 0.0f,
					0.0f };

				//std::cout << "FORCE " << knockbackForce.x << ", " << knockbackForce.y << std::endl;

				// Apply Force
				knockbackForce.x *= 1.5f;
				knockbackForce.y *= 1.5f;

				rb->setVelocity(glm::vec3(
					knockbackForce.x,
					knockbackForce.y,
					0.0f));

				rb->setAcceleration(glm::vec3(
					-knockbackForce.x * 2.0f,
					-knockbackForce.y * 2.0f,
					0.0f));
			}
		};
	}
	else {
		if (ec->getActionState() == IDLE) {
			if ((rand() % 1000) < 5) {
				ec->setActionState(WANDER);
				sr->setAnimActive(1);
				m_wanderTarget = {
				getPosition().x + (rand() % 3) * (((rand() % 100) <= 50) ? 1.0f : -1.0f),
				getPosition().y + (rand() % 3) * (((rand() % 100) <= 50) ? 1.0f : -1.0f),
				getPosition().z };
			}
		}
		else if (ec->getActionState() != WANDER) {
			rb->setVelocity({ 0.0f, 0.0f, 0.0f });
			ec->setActionState(IDLE);
			sr->setAnimActive(0);
		}
	}

	if (ec->getActionState() == WANDER) {
		//std::cout << getObjectID() << " is wandering! Is currently at: " << getPosition().x << ", " << getPosition().y << " aiming for: " << m_wanderTarget.x << ", " << m_wanderTarget.y << std::endl;
		rb->setVelocity({
				((fabs(getPosition().x - m_wanderTarget.x) <= 0.05f) ? 0.0f : (getPosition().x > m_wanderTarget.x) ? -1.0f : 1.0f),
				((fabs(getPosition().y - m_wanderTarget.y) <= 0.05f) ? 0.0f : (getPosition().y > m_wanderTarget.y) ? -1.0f : 1.0f),
				0.0f
		});
		//std::cout << m_objectID << " velocity at: " << m_velocity.x << ", " << m_velocity.y << std::endl;
		if (glm::length(rb->getVelocity()) > 1.0f)
		{
			rb->setVelocity(glm::normalize(rb->getVelocity()));
		}
		rb->setVelocity(rb->getVelocity() * ec->getMoveSpeed() * 0.5f);
		if (getPosition().x > m_wanderTarget.x) {
			setRotation({ getRotation().x, 180.0f, getRotation().z });
		}
		else {
			setRotation({ getRotation().x, 0.0f, getRotation().z });
		}

		for (auto gameObject : *getAllGameObjects()) {
			if (TileMap* tileMap = gameObject->getComponent<TileMap>()) {
				if (tileMap->getCollidable() && (tileMap->getPosition().z == getPosition().z)) {
					if (tileMap->isSolid(m_wanderTarget)) {
						rb->setVelocity({ 0.0f, 0.0f, 0.0f });
						ec->setActionState(IDLE);
						sr->setAnimActive(0);
					}
				}
			}
		}

		if ((glm::distance(getPosition(), m_wanderTarget) <= 0.1f)) {
			rb->setVelocity({ 0.0f, 0.0f, 0.0f });
			ec->setActionState(IDLE);
			sr->setAnimActive(0);
			//std::cout << m_objectID << " reached wanderTarget!" << std::endl;
		}
		if ((getComponent<EntityController>()->getTarget() != nullptr) && (getComponent<EntityController>()->getTarget()->getParent()->isAlive())) {
			if (glm::distance(getPosition(), getComponent<EntityController>()->getTarget()->getPosition()) <= 3.0f) {
				rb->setVelocity({ 0.0f, 0.0f, 0.0f });
				ec->setActionState(IDLE);
				sr->setAnimActive(0);
			}
		}
	}
}