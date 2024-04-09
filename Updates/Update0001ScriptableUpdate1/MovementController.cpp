#include "MovementController.h"

void MovementController::update(float deltaTime) {
	//std::vector<GameMap>& levelMaps, std::vector<Effect>* allEffects, float deltaTime
	if (Entity* entity = m_parent->getComponent<Entity>()) {

		entity->setVelocity(entity->getVelocity() + (entity->getAcceleration() * deltaTime));

		// Check if stunned (knocked back)
		if (entity->getActionState() == STUNNED) {
			if ((entity->getVelocity().x * entity->getAcceleration().x) > 0) {
				entity->setVelocity({ 0.0f, entity->getVelocity().y, entity->getVelocity().z });
				entity->setAcceleration({ 0.0f, entity->getAcceleration().y, entity->getAcceleration().z });
			}
			if ((entity->getVelocity().y * entity->getAcceleration().y) > 0) {
				entity->setVelocity({ entity->getVelocity().x, 0.0f, entity->getVelocity().z });
				entity->setAcceleration({ entity->getAcceleration().x, 0.0f, entity->getAcceleration().z });
			}
			if ((entity->getVelocity().x == 0) && (entity->getVelocity().y == 0) && (entity->getCarrier() == nullptr)) {
				entity->setActionState(IDLE);
				entity->setVelocity({ 0.0f, 0.0f, 0.0f });
			}
		}

		// Check Movement Collision

		if ((entity->getActionState() != FALLING) && (entity->getActionState() != JUMPING) && (entity->getActionState() != STUNNED)) {
			for (auto& gameObject : *getAllGameObjects()) {
				if (GameMap* levelMap = gameObject.getComponent<GameMap>()) {
					if (levelMap->getCollidable()) {
						willCollide(entity, levelMap, levelMap->isPit(), deltaTime);
					}
					if ((levelMap->isPit()) && (levelMap->getPosition().z == entity->getPosition().z)) {
						if (levelMap->isPit(entity->getPosition())) {
							fall(entity, -entity->getPosition().z);
						}
					}
				}
			}
		}

		if ((entity->getActionState() != FALLING) && (entity->getActionState() != JUMPING) && (entity->getActionState() != STUNNED)) {
			if (entity->getHealth() <= 0) {
				diesOfCringe(entity, deltaTime);
			}
		}

		if ((entity->getActionState() == FALLING) || (entity->getActionState() == JUMPING)) {
			//entity->getPosition().z += (entity->getVelocity().y * deltaTime * m_moveSpeed);
			//std::cout << m_objectID << " is at elevation " << entity->getPosition().z << std::endl;
			//std::cout << m_objectID << " is at y " << entity->getPosition().y << " and fallTarget at " << m_fallTarget.y << std::endl;
			//std::cout << m_objectID << " is velo y " << entity->getVelocity().y << " and accel " << m_acceleration.y << std::endl;

			if (entity->getPosition().z + (entity->getVelocity().y * deltaTime * entity->getMoveSpeed()) < entity->getFallTarget().z) {
				entity->setPosition({ entity->getPosition().x, entity->getPosition().y, round(entity->getPosition().z) });

				//std::cout << m_objectID << " landed at elevation " << entity->getPosition().z << std::endl;
				bool overPit = false;
				for (auto& gameObject : *getAllGameObjects()) {
					if (GameMap* levelMap = gameObject.getComponent<GameMap>()) {
						if (levelMap->isPit() && (levelMap->getPosition().z == entity->getPosition().z)) {
							if (levelMap->isPit(entity->getPosition())) {
								overPit = true;
							}
						}
					}
				}

				if (overPit) {
					if (entity->getPosition().z <= 0) {
						//std::cout << m_objectID << " is over Pit " << std::endl;
						diesOfCringe(entity, deltaTime);
					}
					else {
						//std::cout << m_objectID << " Jump land on Pit " << std::endl;
						fall(entity, -entity->getPosition().z);
					}
				}
				else {
					//std::cout << m_objectID << " is not over Pit " << std::endl;

					entity->setActionState(IDLE);
					entity->setPosition(entity->getFallTarget());
					entity->setAcceleration({ entity->getAcceleration().x, 0.0f, entity->getAcceleration().z });
					entity->setVelocity({ 0.0f, 0.0f, 0.0f });
				}
			}
			else {
				entity->setPosition({ entity->getPosition().x,
									  entity->getPosition().y,
									  entity->getPosition().z + (entity->getVelocity().y * deltaTime * entity->getMoveSpeed()) });
			}
		}

		// Move based on Velocity
		entity->setPosition({ entity->getPosition().x + (entity->getVelocity().x * deltaTime * entity->getMoveSpeed()), 
							  entity->getPosition().y + (entity->getVelocity().y * deltaTime * entity->getMoveSpeed()),
							  entity->getPosition().z});

		if (entity->getActionState() == JUMPING) {
			entity->setFallTarget({ entity->getFallTarget().x + (entity->getVelocity().x * deltaTime * entity->getMoveSpeed()), entity->getFallTarget().y, entity->getFallTarget().z });
		}
	}
}

void MovementController::willCollide(Entity* entity, GameMap* gameMap, bool pit, float deltaTime) {
	std::vector<glm::vec3> probes = {
	glm::vec3(entity->getPosition().x, entity->getPosition().y + ((entity->getScale().y / 2) - (entity->getCollisionSize().y / 1.1f)), entity->getPosition().z), // Top
	glm::vec3(entity->getPosition().x - ((entity->getScale().x / 2) - (entity->getCollisionSize().x / 2.0f)), entity->getPosition().y + ((entity->getScale().y / 2) - (entity->getCollisionSize().y / 1.1f)), entity->getPosition().z), // Top Left
	glm::vec3(entity->getPosition().x + ((entity->getScale().x / 2) - (entity->getCollisionSize().x / 2.0f)), entity->getPosition().y + ((entity->getScale().y / 2) - (entity->getCollisionSize().y / 1.1f)), entity->getPosition().z), // Top Right

	glm::vec3(entity->getPosition().x, entity->getPosition().y - ((entity->getScale().y / 2) - (entity->getCollisionSize().y / 2.0f)), entity->getPosition().z), // Bottom
	glm::vec3(entity->getPosition().x - ((entity->getScale().x / 2) - (entity->getCollisionSize().x / 2.0f)), entity->getPosition().y - ((entity->getScale().y / 2) - (entity->getCollisionSize().y / 2.0f)), entity->getPosition().z), // Bottom Left
	glm::vec3(entity->getPosition().x + ((entity->getScale().x / 2) - (entity->getCollisionSize().x / 2.0f)), entity->getPosition().y - ((entity->getScale().y / 2) - (entity->getCollisionSize().y / 2.0f)), entity->getPosition().z), // Bottom Right

	glm::vec3(entity->getPosition().x - ((entity->getScale().x / 2) - (entity->getCollisionSize().x / 2.0f)), entity->getPosition().y, entity->getPosition().z), // Left
	glm::vec3(entity->getPosition().x + ((entity->getScale().x / 2) - (entity->getCollisionSize().x / 2.0f)), entity->getPosition().y, entity->getPosition().z), // Right
	};

	float x_overlap = 0.0f;
	float y_overlap = 0.0f;
	bool x_move = true;
	bool y_move = true;

	for (auto& probe : probes) {
		glm::vec3 newProbePos = { probe.x + entity->getVelocity().x * deltaTime, probe.y, probe.z };
		if ((entity->getPosition().z == gameMap->getPosition().z) && gameMap->isSolid(newProbePos, &x_overlap, &y_overlap) && (x_overlap >= 0) && (y_overlap > 0)) {
			if ((x_overlap < y_overlap)) {
				if (x_move) {
					if (entity->getVelocity().x * (probe.x - entity->getPosition().x) > 0) {
						//std::cout << m_objectID << " attempting to Move into Solid Tile x" << std::endl;
						if ((entity->getActionState() != STUNNED) || !pit) {
							entity->setPosition({ entity->getPosition().x + (entity->getVelocity().x * deltaTime) - (x_overlap * ((entity->getVelocity().x > 0) ? 1.0f : -1.0f)),
												  entity->getPosition().y,
												  entity->getPosition().z });
							entity->setVelocity({ 0.0f,
												  entity->getVelocity().y,
												  entity->getVelocity().z });
						}
					}
					x_move = false;
				}
			}
		}

	}

	for (auto& probe : probes) {
		glm::vec3 newProbePos = { probe.x, probe.y + entity->getVelocity().y * deltaTime, probe.z };
		if ((entity->getPosition().z == gameMap->getPosition().z) && gameMap->isSolid(newProbePos, &x_overlap, &y_overlap) && (x_overlap > 0) && (y_overlap >= 0)) {
			if (y_move) {
				if (entity->getVelocity().y * (probe.y - entity->getPosition().y) > 0) {
					//std::cout << "Attempting to Move into Solid Tile y, x over: " << x_overlap << ", y over: " << y_overlap << std::endl;
					if ((entity->getActionState() != STUNNED) || !pit) {
						entity->setPosition({ entity->getPosition().x,
											  entity->getPosition().y + (entity->getVelocity().y * deltaTime) - (y_overlap * ((entity->getVelocity().y > 0) ? 1.0f : -1.0f)),
											  entity->getPosition().z });
						entity->setVelocity({ entity->getVelocity().x,
											  0.0f,
											  entity->getVelocity().z });
					}
				}
				y_move = false;
			}
		}
	}
}

void MovementController::fall(Entity* entity, float elevationTarget) {
	//std::cout << "------ fall called ------" << m_isAlive <<  std::endl;
	entity->setActionState(FALLING);
	entity->setAnimActive(1);
	entity->setVelocity({ 0.0f,
						  1.0f,
						  entity->getVelocity().z });
	entity->setAcceleration({ 0.0f,
						     -2.0f,
							  entity->getVelocity().z });

	entity->setFallTarget({ entity->getPosition().x,
							entity->getPosition().y,
							roundf(entity->getPosition().z + elevationTarget)
		});
}

void MovementController::diesOfCringe(Entity* entity, float deltaTime) {
	if (m_target != nullptr) {
		if (m_target->getActionState() == STUNNED) {
			if (m_target->getCarrier() == entity) {
				if (MovementController* targetMovementController = m_target->getComponent<MovementController>())
					targetMovementController->fall(m_target, -(entity->getPosition().z));
			}
		}
	}

	entity->setAlive(false);

	/*
	Effect skull = Effect(Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Dead/Dead.png"),
		7, 2, 0, { { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 } }, 0, false);
	skull.setPosition(entity->getPosition());
	skull.setScale({ 0.5f, 0.5f, 1.0f });
	skull.update(deltaTime);
	allEffects->push_back(skull);
	*/
}