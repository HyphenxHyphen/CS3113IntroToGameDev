#include "EntityController.h"

void EntityController::update(float deltaTime) {

	// Check if stunned (knocked back)
	if (m_actionState == STUNNED) {
		if ((rb->getVelocity().x * rb->getAcceleration().x) > 0) {
			rb->setVelocity(X, 0.0f);
			rb->setAcceleration(X, 0.0f);
		}
		if ((rb->getVelocity().y * rb->getAcceleration().y) > 0) {
			rb->setVelocity(Y, 0.0f);
			rb->setAcceleration(Y, 0.0f);
		}
		if ((rb->getVelocity().x == 0) && (rb->getVelocity().y == 0) && (m_carrier == nullptr)) {
			
			//std::cout << m_actionState << std::endl;

			// Check if is over a pit
			//std::cout << "Checking Pit" << std::endl;

			if (isOverPit()) {
				if (getPosition().z <= 0) {
					//std::cout << getObjectID() << " is over Pit " << std::endl;
					diesOfCringe(deltaTime);
				}
				else {
					//std::cout << getObjectID() << " Jump land on Pit " << std::endl;
					fall(-getPosition().z);
				}
			}
			else {
				//std::cout << m_objectID << " is not over Pit " << std::endl;


				m_actionState = IDLE;
				rb->setVelocity({ 0.0f, 0.0f, 0.0f });
				//std::cout << getObjectID() << " is at " << getPosition().x << ", " << getPosition().y << ", " << getPosition().z << std::endl;
				m_parent->removeTag("Airborn");
				rb->removeIgnoreTag("Pit");

			}
		}
	}

	if ((m_actionState != FALLING) && (m_actionState != JUMPING) && (m_actionState != STUNNED)) {
		if (m_health <= 0) {
			//std::cout << getObjectID() << " is GONNA DIE " << std::endl;
			diesOfCringe(deltaTime);
		}
	}

	if ((m_actionState == FALLING) || (m_actionState == JUMPING)) {
		//m_elevation += (m_velocity.y * deltaTime * m_moveSpeed);
		//std::cout << m_objectID << " is at elevation " << m_elevation << std::endl;
		//std::cout << m_objectID << " is at y " << getPosition().y << " and fallTarget at " << m_fallTarget.y << std::endl;
		//std::cout << m_objectID << " is velo y " << m_velocity.y << " and accel " << m_acceleration.y << std::endl;
		//std::cout << getObjectID() << " is at elevation " << getPosition().z << std::endl;

		if (getPosition().z + (rb->getVelocity().y * deltaTime) < m_fallTarget.z) {
			setPosition(Z, round(getPosition().z));

			if (isOverPit()) {
				if (getPosition().z <= 0) {
					//std::cout << getObjectID() << " is over Pit " << std::endl;
					diesOfCringe(deltaTime);
				}
				else {
					//std::cout << getObjectID() << " Jump land on Pit " << std::endl;
					fall(-getPosition().z);
				}
			}
			else {
				//std::cout << m_objectID << " is not over Pit " << std::endl;

				setActionState(IDLE);
				setPosition(m_fallTarget);
				//m_acceleration.y = 0.0f;
				rb->setForce(Y, 0.0f);
				rb->setVelocity({ 0.0f, 0.0f, 0.0f });
				m_parent->removeTag("Airborn");
				rb->removeIgnoreTag("Pit");

			}
		}
		else {
			rb->setPosition(Z, rb->getPosition().z + rb->getVelocity().y * deltaTime);
		}
	}

	if (getActionState() == JUMPING) {
		m_fallTarget.x = (rb->getPosition().x);
		//setPosition(X, getPosition().x + m_velocity.x * deltaTime);

	}

	// If ATTACKING, spawn hitbox on the right frame
	if (m_actionState == ATTACKING) {
		if (SpriteRenderer* spriteRenderer = getComponent<SpriteRenderer>()) {
			if (spriteRenderer->getAnimChangeFlag()) {

				if ((spriteRenderer->getAnimIndex() == 2) || (spriteRenderer->getAnimIndex() == 8)) {
					if (m_atkSFX != nullptr) {
						Mix_PlayChannel(
							-1,       // using the first channel that is not currently in use...
							m_atkSFX,  // ...play this chunk of audio...
							0        // ...once.
						);
					}
				}
				else if ((spriteRenderer->getAnimIndex() == 3) || (spriteRenderer->getAnimIndex() == 9)) {
					std::vector<EntityController*> entitiesHit = {};
					if (m_faceDirection == RIGHT) {
						spawnHitBox(
							glm::vec3( // Hitbox Position
								0.3f,
								0.0f,
								getPosition().z
							),
							glm::vec3( // Hitbox Size
								0.2f,
								0.4f,
								0.0f
							), &entitiesHit);
					}
					else if (m_faceDirection == LEFT) {
						spawnHitBox(
							glm::vec3( // Hitbox Position
								-0.3f,
								0.0f,
								getPosition().z
							),
							glm::vec3( // Hitbox Size
								0.2f,
								0.4f,
								0.0f
							), &entitiesHit);
					}
					else if (m_faceDirection == UP) {
						spawnHitBox(
							glm::vec3( // Hitbox Position
								0.0f,
								0.3f,
								getPosition().z
							),
							glm::vec3( // Hitbox Size
								0.4f,
								0.2f,
								0.0f
							), &entitiesHit);
					}
					else if (m_faceDirection == DOWN) {
						spawnHitBox(
							glm::vec3( // Hitbox Position
								0.0f,
								-0.3f,
								getPosition().z
							),
							glm::vec3( // Hitbox Size
								0.4f,
								0.2f,
								0.0f
							), &entitiesHit);
					}

					EntityController* ec = getComponent<EntityController>();
					// Process entities hit
					for (auto& entityOther : entitiesHit) {
						//std::cout << entity->m_objectID << std::endl;
						entityOther->takeDamage(ec->getAttack());
						entityOther->m_parent->addTag("Airborn");
						entityOther->getComponent<Rigidbody2D>()->addIgnoreTag("Pit");
						if (!entityOther->isEvil() && (getObjectID() == "Player")) {
							//ec->setEvil(true);
							//allEnemiesTrue.insert(allOther.begin(), allOther.end());
							for (auto gameObject : *getAllGameObjects()) {
								if (EntityController * ecSetEvil = gameObject->getComponent<EntityController>()) {
									ecSetEvil->setEvil(true);
								}
							}
						}

						GameObject* damageExplosion = new GameObject("DamageExplosion", getAllGameObjects());
						getAllGameObjects()->push_back(damageExplosion);
						damageExplosion->addComponent<SpriteRenderer>(
							new SpriteRenderer(damageExplosion, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Effects/Explosion/Explosions.png"), 9, 1, 0,
								{ { 0, 1, 2, 3, 4, 5, 6, 7, 8 } }, 0, 12))->setAnimLoop(false);
						damageExplosion->setPosition(entityOther->getPosition());
						damageExplosion->setScale({ 0.5f, 0.5f, 1.0f });

						entityOther->setActionState(STUNNED);

						// Knockback
						glm::vec3 knockbackForce = {
							fabs(getPosition().x - entityOther->getPosition().x),
							fabs(getPosition().y - entityOther->getPosition().y),
							0.0f };
						// Normalize
						//glm::normalize(knockbackForce);
						if (knockbackForce.x > knockbackForce.y) {
							knockbackForce.x /= knockbackForce.x;
							knockbackForce.y /= knockbackForce.x;
						}
						else {
							knockbackForce.x /= knockbackForce.y;
							knockbackForce.y /= knockbackForce.y;
						}
						// Set Direction
						knockbackForce.x *= (getPosition().x > entityOther->getPosition().x ? -1.0f : 1.0f);
						knockbackForce.y *= (getPosition().y > entityOther->getPosition().y ? -1.0f : 1.0f);
						// Apply Force
						knockbackForce.x *= 2.0f;
						knockbackForce.y *= 2.0f;

						entityOther->getComponent<Rigidbody2D>()->setVelocity(glm::vec3(
							knockbackForce.x,
							knockbackForce.y,
							entityOther->getComponent<Rigidbody2D>()->getVelocity().z));

						entityOther->getComponent<Rigidbody2D>()->setAcceleration(glm::vec3(
							-knockbackForce.x,
							-knockbackForce.y,
							0.0f));
					}
				}
			}
		}
	}
}

void EntityController::fall(float elevationTarget) {
	//std::cout << "------ fall called ------" << m_isAlive <<  std::endl;
	setActionState(FALLING);
	getComponent<SpriteRenderer>()->setAnimActive(1);
	rb->setVelocity(X, 0.0f);
	rb->setVelocity(Y, 2.0f);
	rb->setAcceleration(Y, -10.0f);
	m_fallTarget = getPosition();
	m_fallTarget.z += elevationTarget;
	//std::cout << "Fall target: " << m_fallTarget.z << std::endl;
}

void EntityController::jump() {
	//std::cout << "------ jump called ------" << std::endl;
	setActionState(JUMPING);
	getComponent<SpriteRenderer>()->setAnimActive(1);
	rb->setVelocity(Y, 5.0f);
	rb->setAcceleration(Y, -10.0f);
	m_fallTarget = getPosition();
	m_parent->addTag("Airborn");
	rb->addIgnoreTag("Pit");

	if (m_jumpSFX != nullptr) {
		Mix_PlayChannel(
			-1,       // using the first channel that is not currently in use...
			m_jumpSFX,  // ...play this chunk of audio...
			0        // ...once.
		);
	}
}

void EntityController::diesOfCringe(float deltaTime) {
	//std::cout << "I DIED! " << std::endl;
	m_parent->setAlive(false);
	
	if (m_dieSFX != nullptr) {
		Mix_PlayChannel(
			-1,       // using the first channel that is not currently in use...
			m_dieSFX,  // ...play this chunk of audio...
			0        // ...once.
		);
	}

	GameObject* skull = new GameObject("Skull", getAllGameObjects());
	getAllGameObjects()->push_back(skull);
	skull->addComponent<SpriteRenderer>(
		new SpriteRenderer(skull, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Dead/Dead.png"), 7, 2, 0,
			{ { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 } }, 0, 12))->setAnimLoop(false);
	skull->setPosition(getPosition());
	skull->setScale({ 0.5f, 0.5f, 1.0f });
}

void EntityController::spawnHitBox(glm::vec3& hitBoxPosition, glm::vec3& hitBoxSize, std::vector<EntityController*>* entitiesHit) {
	PolygonCollider2D hitbox = PolygonCollider2D(m_parent,
		{ {-hitBoxSize.x / 2,  hitBoxSize.y / 2, 0.0f},
		  { hitBoxSize.x / 2,  hitBoxSize.y / 2, 0.0f},
		  { hitBoxSize.x / 2, -hitBoxSize.y / 2, 0.0f},
		  {-hitBoxSize.x / 2, -hitBoxSize.y / 2, 0.0f}, });
	hitbox.setColliderPosition(hitBoxPosition);

	//std::cout << " Hit Box Called From { " << hitbox.getVertices()[0].x << " to " << hitbox.getVertices()[1].x << ", " << hitbox.getVertices()[0].y << " to " << hitbox.getVertices()[2].y << ", " << hitBoxPosition.z << "}" << std::endl;

	for (auto gameObjectOther : *getAllGameObjects()) {
		if ((gameObjectOther != m_parent) && (gameObjectOther->isAlive())) {
			if (Rigidbody2D * rbOther = gameObjectOther->getComponent<Rigidbody2D>()) {
				if (EntityController * ecOther = gameObjectOther->getComponent<EntityController>()) {
					//std::cout << "Hit? " + rbOther->getObjectID() << " at { " << rbOther->getPosition().x << ", " << rbOther->getPosition().y << ", " << rbOther->getPosition().z << "}" << std::endl;
					for (auto colliderOther : rbOther->getColliders()) {
						CollisionData collisionColliderData = hitbox.isCollidingWith(colliderOther);
						if (collisionColliderData.collided) {
							//std::cout << "Hit " + rbOther->getObjectID() << std::endl;
							entitiesHit->push_back(ecOther);
						}
					}
				}
			}
		}
	}
}

bool EntityController::isOverPit() {
	for (auto gameObject : *getAllGameObjects()) {
		if (TileMap * tileMap = gameObject->getComponent<TileMap>()) {
			if ((tileMap->getParent()->hasTag("Pit")) && (tileMap->getPosition().z == getPosition().z)) {
				if (tileMap->isSolid(getPosition())) {
					return true;
				}
			}
		}
	}
	return false;
}

void const EntityController::takeDamage(float damage) {
	m_health -= damage;
	if (m_hitSFX != nullptr) {
		Mix_PlayChannel(
			-1,       // using the first channel that is not currently in use...
			m_hitSFX,  // ...play this chunk of audio...
			0        // ...once.
		);
	}
}