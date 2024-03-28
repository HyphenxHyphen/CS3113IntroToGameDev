/**
* Author: John Li
* Assignment: Rise of the AI
* Date due: 2024-03-23, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Entity.h"

//GLuint loadTexture(const char* filepath);

Entity::Entity() {
	m_objectID = "Blank";
	m_textureID = NULL;
	m_animCols = 0;
	m_animRows = 0;
	m_animIndex = 0;
	m_anims = {};
	m_animActive = 0;
	m_animTime = 0.0f;

	m_health = 0;
	m_attack = 0;
	m_moveSpeed = 0.0f;

	m_collisionSize = glm::vec3(0.0f);

	m_position = glm::vec3(0.0f);
	m_velocity = glm::vec3(0.0f);
	m_acceleration = glm::vec3(0.0f);

	m_modelMatrix = glm::mat4(1.0f);
}

// Entity.cpp
Entity::Entity(std::string objectID, GLint textureID, unsigned int animCols, unsigned int animRows, unsigned int animIndex, std::vector<std::vector<int>> anims, unsigned int startingAnim, 
	int health, int attack, float moveSpeed, float elevation, glm::vec3 collisionSize)
{
	m_objectID = objectID;
	m_textureID = textureID;
	m_animCols = animCols;
	m_animRows = animRows;
	m_animIndex = animIndex;
	m_anims = anims;
	m_animActive = startingAnim;
	m_animTime = 0.0f;

	m_health = health;
	m_attack = attack;
	m_moveSpeed = moveSpeed;
	m_elevation = elevation;

	setCollisionSize(collisionSize);

	m_velocity = glm::vec3(0.0f);
	m_acceleration = glm::vec3(0.0f);

	m_modelMatrix = glm::mat4(1.0f);
}

Entity::~Entity()
{
	m_collidingWith.clear();
	m_target = nullptr;
	m_carrier = nullptr;
	// Is there a memory leak?
}

void Entity::update(std::vector<GameMap>& levelMaps, std::map<std::string, Entity*>& allOther, std::map<std::string, Entity*>& allEnemiesTrue, std::map<std::string, Entity*>& allEnemies, std::vector<Effect>* allEffects, float deltaTime)
{
	//std::cout << m_objectID << " is in state " << m_actionState << std::endl;

	// Animations
	if ((m_anims.size() > 0) && ((m_actionState != STUNNED) || (m_carrier != nullptr))) {
		if (m_anims[m_animActive].size() > 0) {
			m_animTime += deltaTime;
			float frames_per_second = (float)1 / FRAMES_PER_SECOND;

			if (m_animTime >= frames_per_second) {
				m_animTime = 0.0f;
				m_animIndex++;

				// If ATTACKING, spawn hitbox on the right frame
				if (m_actionState == ATTACKING) {
					if ((m_animIndex == 3) || (m_animIndex == 9)) {
						std::vector<Entity*> entitiesHit = {};
						if (m_faceDirection == RIGHT) {
							spawnHitBox(
								glm::vec3( // Hitbox Position
									m_position.x + 0.3f,
									m_position.y,
									m_position.z
								),
								glm::vec3( // Hitbox Size
									0.10f,
									0.30f,
									0.0f
								),
								allEnemies, &entitiesHit);
						}
						else if (m_faceDirection == LEFT) {
							spawnHitBox(
								glm::vec3( // Hitbox Position
									m_position.x - 0.3f,
									m_position.y,
									m_position.z
								),
								glm::vec3( // Hitbox Size
									0.10f,
									0.30f,
									0.0f
								),
								allEnemies, &entitiesHit);
						}
						else if (m_faceDirection == UP) {
							spawnHitBox(
								glm::vec3( // Hitbox Position
									m_position.x,
									m_position.y + 0.3f,
									m_position.z
								),
								glm::vec3( // Hitbox Size
									0.30f,
									0.10f,
									0.0f
								),
								allEnemies, &entitiesHit);
						}
						else if (m_faceDirection == DOWN) {
							spawnHitBox(
								glm::vec3( // Hitbox Position
									m_position.x,
									m_position.y - 0.3f,
									m_position.z
								),
								glm::vec3( // Hitbox Size
									0.30f,
									0.10f,
									0.0f
								),
								allEnemies, &entitiesHit);
						}
						// Process entities hit
						for (auto& entity : entitiesHit) {
							//std::cout << entity->m_objectID << std::endl;
							entity->takeDamage(m_attack);
							if ((!m_evil) && (!entity->m_evil)) {
								m_evil = true;
								allEnemiesTrue.insert(allOther.begin(), allOther.end());
							}
							Effect damageExplosion = Effect(m_elevation, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Effects/Explosion/Explosions.png"),
								9, 1, 0, { { 0, 1, 2, 3, 4, 5, 6, 7, 8 } }, 0, false);
							damageExplosion.setPosition(entity->getPosition());
							damageExplosion.update(deltaTime);
							allEffects->push_back(damageExplosion);
							entity->setActionState(STUNNED);

							// Knockback
							glm::vec3 knockbackForce = { 
								fabs(m_position.x - entity->getPosition().x),
								fabs(m_position.y - entity->getPosition().y),
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
							knockbackForce.x *= (m_position.x > entity->getPosition().x ? -1.0f : 1.0f);
							knockbackForce.y *= (m_position.y > entity->getPosition().y ? -1.0f : 1.0f);
							// Apply Force
							knockbackForce.x *= 1.0f;
							knockbackForce.y *= 1.0f;

							entity->setVelocity(glm::vec3(
								knockbackForce.x,
								knockbackForce.y,
								entity->getVelocity().z));

							entity->setAcceleration(glm::vec3(
								-knockbackForce.x,
								-knockbackForce.y,
								0.0f));
						}
						

					}
				}


				if (m_animIndex >= m_anims[m_animActive].size()) {
					m_animIndex = 0;
				}
			}
		}
	}

	// Activate AI
	if (m_aiType != NONE) { aiActivate(levelMaps, deltaTime); }

	// Initial velocity
	//m_velocity.x = m_movement.x * m_moveSpeed;

	m_velocity += m_acceleration * deltaTime;

	// Check if stunned (knocked back)
	if (m_actionState == STUNNED) {
		if ((m_velocity.x * m_acceleration.x) > 0) {
			m_velocity.x = 0;
			m_acceleration.x = 0;
		}
		if ((m_velocity.y * m_acceleration.y) > 0) {
			m_velocity.y = 0;
			m_acceleration.y = 0;
		}
		if ((m_velocity.x == 0) && (m_velocity.y == 0) && (m_carrier == nullptr)) {
			m_actionState = IDLE;
			m_velocity = { 0.0f, 0.0f, 0.0f };

			//std::cout << m_actionState << std::endl;

			// Check if is over a pit
			//std::cout << "Checking Pit" << std::endl;
		}
	}

	// Check Movement Collision
	if ((m_actionState != FALLING) && (m_actionState != JUMPING)) {
		for (auto& levelMap : levelMaps) {
			if (levelMap.getCollidable()) {
				willCollide(&levelMap, levelMap.isPit(), deltaTime);
			}
		}
	}

	if ((m_actionState == FALLING) || (m_actionState == JUMPING)) {
		//m_elevation += (m_velocity.y * deltaTime * m_moveSpeed);
		//std::cout << m_objectID << " is at elevation " << m_elevation << std::endl;
		//std::cout << m_objectID << " is at y " << m_position.y << " and fallTarget at " << m_fallTarget.y << std::endl;
		//std::cout << m_objectID << " is velo y " << m_velocity.y << " and accel " << m_acceleration.y << std::endl;

		if (m_position.y + (m_velocity.y * deltaTime * m_moveSpeed) < m_fallTarget.y) {
			m_elevation = round(m_elevation);

			//std::cout << m_objectID << " landed at elevation " << m_elevation << std::endl;
			bool overPit = false;
			for (auto& levelMap : levelMaps) {
				if (levelMap.isPit() && (levelMap.m_elevation == m_elevation)) {
					if (levelMap.isPit(m_position)) {
						overPit = true;
					}
				}
			}

			if (overPit) {
				if (m_actionState == FALLING) {
					//std::cout << m_objectID << " is over Pit " << std::endl;

					m_elevation = 0.0f;
					diesOfCringe(allEffects, deltaTime);
				}
				else if (m_actionState == JUMPING) {
					//std::cout << m_objectID << " Jump land on Pit " << std::endl;
					fall();
				}
			}
			else {
				//std::cout << m_objectID << " is not over Pit " << std::endl;

				m_actionState = IDLE;
				m_position = m_fallTarget;
				m_acceleration.y = 0.0f;
				m_velocity = { 0.0f, 0.0f, 0.0f };
			}
		}
		else {
			m_elevation += (m_velocity.y * deltaTime * m_moveSpeed);
		}
	}

	if ((m_actionState != FALLING) && (m_actionState != JUMPING) && (m_actionState != STUNNED)) {
		for (auto& levelMap : levelMaps) {
			if (levelMap.getCollidable() && levelMap.isPit()) {
				if (levelMap.isPit(m_position)) {
					fall();
				}
			}
		}

		if (m_health <= 0) {
			diesOfCringe(allEffects, deltaTime);
		}
	}

	// Move based on Velocity
	m_position.x += (m_velocity.x * deltaTime * m_moveSpeed);
	m_position.y += (m_velocity.y * deltaTime * m_moveSpeed);

	if (m_actionState == JUMPING) {
		m_fallTarget.x += (m_velocity.x * deltaTime * m_moveSpeed);
	}
	// Apply transformation
	m_modelMatrix = glm::mat4(1.0f);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
}

void Entity::render(ShaderProgram* program) {
	program->set_model_matrix(m_modelMatrix);
	if ((m_anims.size() > 0) && (m_anims[m_animActive].size() > 0)) {
		drawFromAtlas(program, m_textureID, m_anims[m_animActive][m_animIndex]);
	}
	else if ((m_animRows > 0) && (m_animCols > 0)) {
		drawFromAtlas(program, m_textureID, m_animIndex);
	}
}

void Entity::drawFromAtlas(ShaderProgram* program, GLuint textureID, int index)
{
	// Step 1: Calculate the UV location of the indexed frame
	float uCoord = (float)(index % m_animCols) / (float)m_animCols;
	float vCoord = (float)(index / m_animCols) / (float)m_animRows;

	// Step 2: Calculate its UV size
	float width = 1.0f / (float)m_animCols;
	float height = 1.0f / (float)m_animRows;

	// Step 3: Just as we have done before, match the texture coordinates to the vertices
	float textureCoordinates[] =
	{
		uCoord, vCoord + height, uCoord + width, vCoord + height, uCoord + width, vCoord,
		uCoord, vCoord + height, uCoord + width, vCoord, uCoord, vCoord
	};

	float vertices[] =
	{
		-0.5, -0.5, 0.5, -0.5,  0.5, 0.5,
		-0.5, -0.5, 0.5,  0.5, -0.5, 0.5
	};

	// Step 4: And render
	glBindTexture(GL_TEXTURE_2D, textureID);

	glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->get_position_attribute());

	glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, textureCoordinates);
	glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->get_position_attribute());
	glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}

void Entity::checkCollision(std::map<std::string, Entity*>& allEntities) {
	checkColliding(allEntities);

	for (auto& collidingWith : m_collidingWith) {
		//std::cout << m_objectID << " is colliding with " << collidingWith.second->m_objectID << std::endl;

		float x_distance = fabs(m_position.x - collidingWith.second->m_position.x);
		float x_overlap = fabs(x_distance - (m_collisionSize.x / 2.0f) - (collidingWith.second->m_collisionSize.x / 2.0f));

		float y_distance = fabs(m_position.y - collidingWith.second->m_position.y);
		float y_overlap = fabs(y_distance - (m_collisionSize.y / 2.0f) - (collidingWith.second->m_collisionSize.y / 2.0f));

		if ((x_overlap < y_overlap) && ((x_overlap > 0))) {
			m_position.x += x_overlap * ((m_position.x > collidingWith.second->m_position.x) ? 1.0f : -1.0f);
			m_velocity.x = 0.0f;
		}
		else if(y_overlap > 0) {
			m_position.y += y_overlap * ((m_position.y > collidingWith.second->m_position.y) ? 1.0f : -1.0f);
			m_velocity.y = 0.0f;
		}
	}

	cleanCollidingWith();
}

// UNUSED
void Entity::checkCollision(GameMap* gameMap) {
	// Probes for tiles above
	std::vector<glm::vec3> probes = {
	glm::vec3(m_position.x, m_position.y + ((m_scale.y / 2) - (m_collisionSize.y / 2.0f)), m_position.z), // Top
	glm::vec3(m_position.x - ((m_scale.x / 2) - (m_collisionSize.x / 2.0f)), m_position.y + ((m_scale.y / 2) - (m_collisionSize.y / 2.0f)), m_position.z), // Top Left
	glm::vec3(m_position.x + ((m_scale.x / 2) - (m_collisionSize.x / 2.0f)), m_position.y + ((m_scale.y / 2) - (m_collisionSize.y / 2.0f)), m_position.z), // Top Right

	glm::vec3(m_position.x, m_position.y - ((m_scale.y / 2) - (m_collisionSize.y / 2.0f)), m_position.z), // Bottom
	glm::vec3(m_position.x - ((m_scale.x / 2) - (m_collisionSize.x / 2.0f)), m_position.y - ((m_scale.y / 2) - (m_collisionSize.y / 2.0f)), m_position.z), // Bottom Left
	glm::vec3(m_position.x + ((m_scale.x / 2) - (m_collisionSize.x / 2.0f)), m_position.y - ((m_scale.y / 2) - (m_collisionSize.y / 2.0f)), m_position.z), // Bottom Right

	glm::vec3(m_position.x - ((m_scale.x / 2) - (m_collisionSize.x / 2.0f)), m_position.y, m_position.z), // Left
	glm::vec3(m_position.x + ((m_scale.x / 2) - (m_collisionSize.x / 2.0f)), m_position.y, m_position.z), // Right
	};

	float x_overlap = 0.0f;
	float y_overlap = 0.0f;
	bool x_move = true;
	bool y_move = true;

	for (auto& probe : probes) {
		if (gameMap->isSolid(probe, &x_overlap, &y_overlap) && (x_overlap > 0) && (y_overlap > 0)) {
			if ((x_overlap < y_overlap)) {
				if (x_move) {
					//std::cout << "true set x, velo x: " << m_velocity.x << ", probe calc: " << (probe.x - m_position.x) << std::endl;
					m_position.x += x_overlap * ((m_position.x > probe.x) ? 1.0f : -1.0f);
					if (m_velocity.x * (probe.x - m_position.x) > 0) {
						//std::cout << "set x" << std::endl;
						m_velocity.x = 0.0f;
					}
					x_move = false;
				}
			}
			else {
				if (y_move) {
					m_position.y += y_overlap * ((m_position.y > probe.y) ? 1.0f : -1.0f);
					if (m_velocity.y * (probe.y - m_position.y) > 0) {
						m_velocity.y = 0.0f;
					}
					y_move = false;
				}
			}
		}
	}
}


void Entity::willCollide(GameMap* gameMap, bool pit, float deltaTime) {
	std::vector<glm::vec3> probes = {
	glm::vec3(m_position.x, m_position.y + ((m_scale.y / 2) - (m_collisionSize.y / 1.1f)), m_position.z), // Top
	glm::vec3(m_position.x - ((m_scale.x / 2) - (m_collisionSize.x / 2.0f)), m_position.y + ((m_scale.y / 2) - (m_collisionSize.y / 1.1f)), m_position.z), // Top Left
	glm::vec3(m_position.x + ((m_scale.x / 2) - (m_collisionSize.x / 2.0f)), m_position.y + ((m_scale.y / 2) - (m_collisionSize.y / 1.1f)), m_position.z), // Top Right

	glm::vec3(m_position.x, m_position.y - ((m_scale.y / 2) - (m_collisionSize.y / 2.0f)), m_position.z), // Bottom
	glm::vec3(m_position.x - ((m_scale.x / 2) - (m_collisionSize.x / 2.0f)), m_position.y - ((m_scale.y / 2) - (m_collisionSize.y / 2.0f)), m_position.z), // Bottom Left
	glm::vec3(m_position.x + ((m_scale.x / 2) - (m_collisionSize.x / 2.0f)), m_position.y - ((m_scale.y / 2) - (m_collisionSize.y / 2.0f)), m_position.z), // Bottom Right

	glm::vec3(m_position.x - ((m_scale.x / 2) - (m_collisionSize.x / 2.0f)), m_position.y, m_position.z), // Left
	glm::vec3(m_position.x + ((m_scale.x / 2) - (m_collisionSize.x / 2.0f)), m_position.y, m_position.z), // Right
	};

	float x_overlap = 0.0f;
	float y_overlap = 0.0f;
	bool x_move = true;
	bool y_move = true;

	for (auto& probe : probes) {
		glm::vec3 newProbePos = { probe.x + m_velocity.x * deltaTime, probe.y, probe.z };
		if (gameMap->isSolid(newProbePos, &x_overlap, &y_overlap) && (x_overlap >= 0) && (y_overlap > 0)) {
			if ((x_overlap < y_overlap)) {
				if (x_move) {
					if (m_velocity.x * (probe.x - m_position.x) > 0) {
						//std::cout << m_objectID << " attempting to Move into Solid Tile x" << std::endl;
						if ((m_actionState != STUNNED) || !pit)  {
							m_position.x += (m_velocity.x * deltaTime) - (x_overlap * ((m_velocity.x > 0) ? 1.0f : -1.0f));
							m_velocity.x = 0;
						}
					}
					x_move = false;
				}
			}
		}

	}

	for (auto& probe : probes) {
		glm::vec3 newProbePos = { probe.x, probe.y + m_velocity.y * deltaTime, probe.z };
		if (gameMap->isSolid(newProbePos, &x_overlap, &y_overlap) && (x_overlap > 0) && (y_overlap >= 0)) {
			if (y_move) {
				if (m_velocity.y * (probe.y - m_position.y) > 0) {
					//std::cout << "Attempting to Move into Solid Tile y, x over: " << x_overlap << ", y over: " << y_overlap << std::endl;
					if ((m_actionState != STUNNED) || !pit) {
						m_position.y += (m_velocity.y * deltaTime) - (y_overlap * ((m_velocity.y > 0) ? 1.0f : -1.0f));
						m_velocity.y = 0;
					}
				}
				y_move = false;
			}
		}
	}
}

void Entity::fall() {
	//std::cout << "------ fall called ------" << m_isAlive <<  std::endl;
	m_actionState = FALLING;
	setAnimActive(1);
	m_velocity.x = 0.0f;
	m_velocity.y = 1.0f;
	m_acceleration.x = 0.0f;
	m_acceleration.y = -2.0f;
	m_fallTarget = m_position;
	m_fallTarget.y -= m_elevation;
}


void Entity::checkColliding(std::map<std::string, Entity*>& allEntities)
{
	for (auto& entityPair : allEntities) {
		if (entityPair.first != m_objectID) {
			if (isCollidingWith(entityPair.second) && (m_collidingWith.count(entityPair.first) == 0)) {
				m_collidingWith[entityPair.first] = entityPair.second;
			}
			else if (!isCollidingWith(entityPair.second) && (m_collidingWith.count(entityPair.first) >= 1)) {
				m_collidingWith.erase(entityPair.first);
			}
		}
	}
}

void Entity::cleanCollidingWith()
{
	for (auto& entityPair : m_collidingWith) {
		if (entityPair.first != m_objectID) {
			if (!isCollidingWith(entityPair.second)) {
				m_collidingWith.erase(entityPair.first);
			}
		}
	}
}

bool Entity::isCollidingWith(const Entity* entity) {
	float xColDistance = fabs(m_position.x - entity->m_position.x) - ((m_collisionSize.x + entity->m_collisionSize.x) / 2.0f);
	float yColDistance = fabs(m_position.y - entity->m_position.y) - ((m_collisionSize.y + entity->m_collisionSize.y) / 2.0f);

	if ((xColDistance < 0.0f) && (yColDistance < 0.0f)) {
		return true;
	}

	return false;
}

void Entity::spawnHitBox(glm::vec3& hitBoxPosition, glm::vec3& hitBoxSize, std::map<std::string, Entity*>& allEntities, std::vector<Entity*>* entitiesHit) {
	for (auto& entityPair : allEntities) {
		float xColDistance = fabs(hitBoxPosition.x - entityPair.second->m_position.x) - ((hitBoxSize.x + entityPair.second->m_collisionSize.x) / 2.0f);
		float yColDistance = fabs(hitBoxPosition.y - entityPair.second->m_position.y) - ((hitBoxSize.y + entityPair.second->m_collisionSize.y) / 2.0f);

		if ((xColDistance < 0.0f) && (yColDistance < 0.0f) && (entityPair.second->m_actionState != JUMPING) && (entityPair.second->m_actionState != FALLING)) {
			entitiesHit->push_back(entityPair.second);
		}
	}
}

void Entity::diesOfCringe(std::vector<Effect>* allEffects, float deltaTime) {
	m_isAlive = false;
	Effect skull = Effect(m_elevation, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Dead/Dead.png"),
		7, 2, 0, { { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 } }, 0, false);
	skull.m_elevation = m_elevation;
	skull.setPosition(m_position);
	skull.setScale({ 0.5f, 0.5f, 1.0f });
	skull.update(deltaTime);
	allEffects->push_back(skull);
}

void Entity::jump() {
	//std::cout << "------ jump called ------" << std::endl;

	m_actionState = JUMPING;
	setAnimActive(1);
	m_velocity.y = 2.0f;
	m_acceleration.y = -3.0f;
	m_fallTarget = m_position;
}

void Entity::aiActivate(std::vector<GameMap>& levelMaps, float deltaTime)
{
	switch (m_aiType)
	{
	case GUARD:
		aiGuard(deltaTime);
		break;
	case VILLAGER:
		aiVillager(levelMaps, deltaTime);
		break;
	case GOBLIN:
		aiGoblin(levelMaps, deltaTime);
		break;
	}
}

void Entity::aiGuard(float deltaTime)
{
	if (m_target == nullptr) {
		if (m_actionState != IDLE) {
			m_actionState = IDLE;
			m_velocity = { 0.0f, 0.0f, 0.0f };
			m_acceleration = { 0.0f, 0.0f, 0.0f };
			setAnimActive(0);
		}
		return;
	}

	if ((m_health < 5) || (m_target->m_evil)) {
		glm::vec3 targetPos = m_target->getPosition();
		if ((m_target->m_actionState == JUMPING) || (m_target->m_actionState == FALLING)) {
			targetPos = m_target->m_fallTarget;
		}

		if (m_actionState == IDLE) {
			if (glm::distance(m_position, m_target->getPosition()) <= 3.0f) {
				if (((m_target->m_actionState != JUMPING) && (m_target->m_actionState != FALLING)) || (glm::distance(m_position, targetPos) > 0.5f)) {
					m_actionState = MOVING;
					setAnimActive(1);
				}
			}

			if (((m_target->m_actionState == JUMPING) || (m_target->m_actionState == FALLING)) && (m_target->getVelocity().y < 0)) {
				//std::cout << "target is about to fall!" << std::endl;
				if (glm::distance(m_position, m_target->m_fallTarget) <= 0.5f) {
					//std::cout << "I am near target fallTarget!" << std::endl;
					if (glm::distance(m_target->m_fallTarget, m_target->getPosition()) <= 0.9f) {
						//std::cout << "Target is near fallTarget!" << std::endl;
						// Fall Prediction
						m_actionState = ATTACKING;
						if (fabs(m_position.x - m_target->getPosition().x) < fabs(m_position.y - m_target->getPosition().y)) {
							if (m_position.x < m_target->getPosition().x) {
								m_faceDirection = RIGHT;
								m_rotation = { m_rotation.x, 0.0f, m_rotation.z };
								setAnimActive(2);
							}
							else {
								m_faceDirection = LEFT;
								m_rotation = { m_rotation.x, 180.0f, m_rotation.z };
								setAnimActive(2);
							}
						}
						else {
							if (m_position.y > m_target->getPosition().y) {
								m_faceDirection = DOWN;
								setAnimActive(3);
							}
							else {
								m_faceDirection = UP;
								setAnimActive(4);
							}
						}
					}
				}
			}
		}

		if (m_actionState == MOVING) {
			//std::cout << "------ move called ------" << std::endl;
			m_velocity = {
				((fabs(m_position.x - targetPos.x) <= 0.1f) ? 0.0f : (m_position.x > targetPos.x) ? -1.0f : 1.0f) * m_moveSpeed,
				((fabs(m_position.y - targetPos.y) <= 0.1f) ? 0.0f : (m_position.y > targetPos.y) ? -1.0f : 1.0f) * m_moveSpeed,
				0.0f
			};
			if (glm::length(m_velocity) > 1.0f)
			{
				m_velocity = glm::normalize(m_velocity);
			}
			if (m_position.x > targetPos.x) {
				m_rotation = { m_rotation.x, 180.0f, m_rotation.z };
			}
			else {
				m_rotation = { m_rotation.x, 0.0f, m_rotation.z };
			}
			if (glm::distance(m_position, targetPos) > 4.0f) {
				m_velocity = { 0.0f, 0.0f, 0.0f };
				m_actionState = IDLE;
				setAnimActive(0);
			}
			if ((glm::distance(m_position, targetPos) <= 0.25f)) {
				m_velocity = { 0.0f, 0.0f, 0.0f };
				m_actionState = IDLE;
				setAnimActive(0);
			}

			if ((glm::distance(m_position, m_target->getPosition()) <= 0.5f) || 
				(glm::distance(m_position, (m_target->getPosition() + (m_target->getVelocity() * deltaTime * 22.0f))) <= 0.5f)) { // Prefiring Prediction for 22 frames
				m_velocity = { 0.0f, 0.0f, 0.0f };
				m_actionState = ATTACKING;
				if (fabs(m_position.x - m_target->getPosition().x) > fabs(m_position.y - m_target->getPosition().y)) {
					if (m_position.x < m_target->getPosition().x) {
						m_faceDirection = RIGHT;
						m_rotation = { m_rotation.x, 0.0f, m_rotation.z };
						setAnimActive(2);
					}
					else {
						m_faceDirection = LEFT;
						m_rotation = { m_rotation.x, 180.0f, m_rotation.z };
						setAnimActive(2);
					}
				}
				else {
					if (m_position.y > m_target->getPosition().y) {
						m_faceDirection = DOWN;
						setAnimActive(3);
					}
					else {
						m_faceDirection = UP;
						setAnimActive(4);
					}
				}
			}
		}

		if (m_actionState == ATTACKING) {
			if ((m_animIndex == 5) || (m_animIndex == 11)) {
				if (glm::distance(m_position, m_target->getPosition()) > 0.5f) {
					m_actionState = IDLE;
					m_velocity = { 0.0f, 0.0f, 0.0f };
					setAnimActive(0);
				}
			}
		};
	}
}

void Entity::aiVillager(std::vector<GameMap>& levelMaps, float deltaTime)
{
	//std::cout << m_objectID << " actionState is: " << m_actionState << std::endl;

	if (m_target != nullptr) {
		glm::vec3 targetPos = m_target->getPosition();
		if ((m_target->m_actionState == JUMPING) || (m_target->m_actionState == FALLING)) {
			targetPos = m_target->m_fallTarget;
		}

		if (m_actionState == IDLE) {
			if ((glm::distance(m_position, targetPos) <= 2.0f) && (m_target->m_evil)) {
				m_actionState = MOVING;
				setAnimActive(4);
			}
			else if ((rand() % 1000) < 5) {
				m_actionState = WANDER;
				setAnimActive(1);
				m_wanderTarget = {
				m_position.x + (rand() % 3) * (((rand() % 100) <= 50) ? 1.0f : -1.0f),
				m_position.y + (rand() % 3) * (((rand() % 100) <= 50) ? 1.0f : -1.0f),
				0.0f };
				for (auto& levelMap : levelMaps) {
					if (levelMap.getCollidable() && levelMap.isPit()) {
						if (levelMap.isPit(m_wanderTarget)) {
							m_wanderTarget = {
							m_position.x + (rand() % 3) * (((rand() % 100) <= 50) ? 1.0f : -1.0f),
							m_position.y + (rand() % 3) * (((rand() % 100) <= 50) ? 1.0f : -1.0f),
							0.0f };
						}
					}
				}
			}
		}

		if (m_actionState == MOVING) {
			if ((glm::distance(m_position, targetPos) <= 2.0f) && (m_target->m_evil)) { // He's evil and nearby! Run away!
				m_velocity = {
				((m_position.x > targetPos.x) ? 1.0f : -1.0f) * m_moveSpeed,
				((m_position.y > targetPos.y) ? 1.0f : -1.0f) * m_moveSpeed,
				0.0f
				};

				if (m_position.x > targetPos.x) {
					m_rotation = { m_rotation.x, 0.0f, m_rotation.z };
				}
				else {
					m_rotation = { m_rotation.x, 180.0f, m_rotation.z };
				}
			}
			if ((glm::distance(m_position, m_target->getPosition()) <= 0.5f) && (m_target->m_evil) && (m_target->m_carrier == nullptr)) { // He's too close! Rush him!
				m_velocity = {
				((fabs(m_position.x - targetPos.x) <= 0.1f) ? 0.0f : (m_position.x > targetPos.x) ? -1.0f : 1.0f) * m_moveSpeed * 1.5f,
				((fabs(m_position.y - targetPos.y) <= 0.1f) ? 0.0f : (m_position.y > targetPos.y) ? -1.0f : 1.0f) * m_moveSpeed * 1.5f,
				0.0f
				};

				if (m_position.x < targetPos.x) {
					m_rotation = { m_rotation.x, 0.0f, m_rotation.z };
				}
				else {
					m_rotation = { m_rotation.x, 180.0f, m_rotation.z };
				}
			}
			//std::cout << m_objectID << " velocity at: " << m_velocity.x << ", " << m_velocity.y << std::endl;

			if (glm::length(m_velocity) > 1.0f)
			{
				m_velocity = glm::normalize(m_velocity);
			}

			if (glm::distance(m_position, targetPos) > 4.0f) {
				m_velocity = { 0.0f, 0.0f, 0.0f };
				m_actionState = IDLE;
				setAnimActive(0);
			}
			else if (glm::distance(m_position, m_target->getPosition()) <= 0.25f) {

				float xColDistance = fabs(m_position.x - m_target->getPosition().x) - ((m_collisionSize.x + m_target->getCollisionSize().x) / 2.0f);
				float yColDistance = fabs(m_position.y - m_target->getPosition().y) - ((m_collisionSize.y + m_target->getCollisionSize().y) / 2.0f);

				if ((xColDistance < 0.0f) && (yColDistance < 0.0f) && (m_target->m_actionState != JUMPING) && (m_target->m_actionState != FALLING) && (m_target->m_carrier == nullptr)) {
					//std::cout << m_objectID << " begin carrying " << m_target->m_objectID << std::endl;

					m_target->setActionState(STUNNED);
					//std::cout << m_target->m_objectID << " is now " << m_target->m_actionState << std::endl;
					m_target->setAnimActive(1);
					m_target->m_elevation = m_elevation += 0.25;


					m_target->m_carrier = this;
					m_velocity = { 0.0f, 0.0f, 0.0f };
					m_actionState = CARRY;
					setAnimActive(4);
				}
			}
		}

		if (m_actionState == CARRY) {
			if (m_target->m_actionState == STUNNED) {
				m_target->setPosition({
					m_position.x,
					m_position.y + 0.25f,
					m_position.z });

				m_velocity = {
				1.0f * m_moveSpeed,
				0.0f * m_moveSpeed,
				0.0f
				};

				m_rotation = { m_rotation.x, 0.0f, m_rotation.z };

				if (glm::length(m_velocity) > 1.0f)
				{
					m_velocity = glm::normalize(m_velocity);
				}


				float x_overlap = 0.0f;
				float y_overlap = 0.0f;
				bool x_move = true;
				glm::vec3 probe = glm::vec3(m_position.x + ((m_scale.x / 2) - (m_collisionSize.x / 2.0f)) + m_velocity.x * deltaTime, m_position.y, m_position.z); // Right
				for (auto& levelMap : levelMaps) {
					if (levelMap.isPit() && levelMap.isSolid(probe, &x_overlap, &y_overlap) && (x_overlap > 0) && (y_overlap >= 0)) {
						//std::cout << "Attempting to Move into Solid Tile y, x over: " << x_overlap << ", y over: " << y_overlap << std::endl;
						if (x_move) {
							if (m_velocity.x * (probe.x - m_position.x) > 0) {
								jump();
								//std::cout << "Attempting to Move into Solid Tile y, x over: " << x_overlap << ", y over: " << y_overlap << std::endl;
								/*
								if ((m_actionState != STUNNED) || !levelMap.isPit()) {
									m_position.y += (m_velocity.y * deltaTime) - (y_overlap * ((m_velocity.y > 0) ? 1.0f : -1.0f));
									m_velocity.y = 0;
								}
								*/
							}
							x_move = false;
						}
					}
				}
			}
			else {

				m_velocity = { 0.0f, 0.0f, 0.0f };
				m_actionState = IDLE;
				setAnimActive(0);
				//std::cout << m_target->m_objectID << " unstunned!" << std::endl;
			}
		}

		if (m_actionState == JUMPING) {
			if (m_target->m_actionState == STUNNED) {
				if (m_target->m_carrier == this) {
					m_target->setPosition({
					m_position.x,
					m_position.y + 0.25f,
					m_position.z });
				}
			}
		}

		if (m_actionState == FALLING) {
			if (m_target->m_actionState == STUNNED) {
				if (m_target->m_carrier == this) {
					m_target->setActionState(IDLE);
				}
			}
		}
	}
	else {
		if (m_actionState == IDLE) {
			if ((rand() % 1000) < 5) {
				m_actionState = WANDER;
				setAnimActive(1);
				m_wanderTarget = {
				m_position.x + (rand() % 3) * (((rand() % 100) <= 50) ? 1.0f : -1.0f),
				m_position.y + (rand() % 3) * (((rand() % 100) <= 50) ? 1.0f : -1.0f),
				0.0f };
			}
		}
	}

	if (m_actionState == WANDER) {
		//std::cout << m_objectID << " is wandering! Is currently at: " << m_position.x << ", " << m_position.y << " aiming for: " << m_wanderTarget.x << ", " << m_wanderTarget.y << std::endl;
		m_velocity = {
				((fabs(m_position.x - m_wanderTarget.x) <= 0.05f) ? 0.0f : (m_position.x > m_wanderTarget.x) ? -1.0f : 1.0f) * m_moveSpeed * 0.5f,
				((fabs(m_position.y - m_wanderTarget.y) <= 0.05f) ? 0.0f : (m_position.y > m_wanderTarget.y) ? -1.0f : 1.0f) * m_moveSpeed * 0.5f,
				0.0f
		};
		//std::cout << m_objectID << " velocity at: " << m_velocity.x << ", " << m_velocity.y << std::endl;
		if (glm::length(m_velocity) > 1.0f)
		{
			m_velocity = glm::normalize(m_velocity);
		}
		if (m_position.x > m_wanderTarget.x) {
			m_rotation = { m_rotation.x, 180.0f, m_rotation.z };
		}
		else {
			m_rotation = { m_rotation.x, 0.0f, m_rotation.z };
		}
		for (auto& levelMap : levelMaps) {
			float x_overlap = 0.0f;
			float y_overlap = 0.0f;
			if (levelMap.getCollidable()) {
				if ((levelMap.isSolid(m_wanderTarget, &x_overlap, &y_overlap) && (x_overlap > 0) && (y_overlap > 0)) || (levelMap.isPit(m_wanderTarget))) {
					m_velocity = { 0.0f, 0.0f, 0.0f };
					m_actionState = IDLE;
					setAnimActive(0);
					//std::cout << m_objectID << "'s wanderTarget is inaccessible!" << std::endl;
				}
			}
		}
		if ((glm::distance(m_position, m_wanderTarget) <= 0.1f)) {
			m_velocity = { 0.0f, 0.0f, 0.0f };
			m_actionState = IDLE;
			setAnimActive(0);
			//std::cout << m_objectID << " reached wanderTarget!" << std::endl;
		}
		if ((m_target != nullptr) && (glm::distance(m_position, m_target->getPosition()) <= 2.0f) && (m_target->m_evil)) {
			m_actionState = MOVING;
			setAnimActive(4);
		}
	}
}

void Entity::aiGoblin(std::vector<GameMap>& levelMaps, float deltaTime)
{
	if (m_target != nullptr) {
		glm::vec3 targetPos = m_target->getPosition();
		if ((m_target->m_actionState == JUMPING) || (m_target->m_actionState == FALLING)) {
			targetPos = m_target->m_fallTarget;
		}

		if (m_actionState == IDLE) {
			if ((rand() % 1000) < 5) {
				m_actionState = WANDER;
				setAnimActive(1);
				m_wanderTarget = {
				m_position.x + (rand() % 3) * (((rand() % 100) <= 50) ? 1.0f : -1.0f),
				m_position.y + (rand() % 3) * (((rand() % 100) <= 50) ? 1.0f : -1.0f),
				0.0f };
			}
		}

		if ((m_actionState == IDLE) && ((m_target->m_actionState != ATTACKING) || (glm::distance(m_position, m_target->getPosition()) <= 0.5f))) {
			if (glm::distance(m_position, m_target->getPosition()) <= 3.0f) {
				if (((m_target->m_actionState != JUMPING) && (m_target->m_actionState != FALLING)) || (glm::distance(m_position, targetPos) > 0.5f)) {
					m_actionState = MOVING;
					setAnimActive(1);
				}
			}

			if (((m_target->m_actionState == JUMPING) || (m_target->m_actionState == FALLING)) && (m_target->getVelocity().y < 0)) {
				//std::cout << "target is about to fall!" << std::endl;
				if (glm::distance(m_position, m_target->m_fallTarget) <= 0.5f) {
					//std::cout << "I am near target fallTarget!" << std::endl;
					if (glm::distance(m_target->m_fallTarget, m_target->getPosition()) <= 0.9f) {
						//std::cout << "Target is near fallTarget!" << std::endl;
						// Fall Prediction
						m_actionState = ATTACKING;
						if (fabs(m_position.x - m_target->getPosition().x) < fabs(m_position.y - m_target->getPosition().y)) {
							if (m_position.x < m_target->getPosition().x) {
								m_faceDirection = RIGHT;
								m_rotation = { m_rotation.x, 0.0f, m_rotation.z };
								setAnimActive(2);
							}
							else {
								m_faceDirection = LEFT;
								m_rotation = { m_rotation.x, 180.0f, m_rotation.z };
								setAnimActive(2);
							}
						}
						else {
							if (m_position.y > m_target->getPosition().y) {
								m_faceDirection = DOWN;
								setAnimActive(3);
							}
							else {
								m_faceDirection = UP;
								setAnimActive(4);
							}
						}
					}
				}
			}
		}

		if (m_actionState == MOVING) {
			//std::cout << "------ move called ------" << std::endl;
			m_velocity = {
				((fabs(m_position.x - targetPos.x) <= 0.1f) ? 0.0f : (m_position.x > targetPos.x) ? -1.0f : 1.0f) * m_moveSpeed * (((m_target->m_actionState == ATTACKING) && (glm::distance(m_position, m_target->getPosition()) <= 0.5f)) ? -1.0f : 1.0f),
				((fabs(m_position.y - targetPos.y) <= 0.1f) ? 0.0f : (m_position.y > targetPos.y) ? -1.0f : 1.0f) * m_moveSpeed * (((m_target->m_actionState == ATTACKING) && (glm::distance(m_position, m_target->getPosition()) <= 0.5f)) ? -1.0f : 1.0f),
				0.0f
			};
			if (glm::length(m_velocity) > 1.0f)
			{
				m_velocity = glm::normalize(m_velocity);
			}
			if (m_velocity.x < 0) {
				m_rotation = { m_rotation.x, 180.0f, m_rotation.z };
			}
			else {
				m_rotation = { m_rotation.x, 0.0f, m_rotation.z };
			}
			if (glm::distance(m_position, targetPos) > 4.0f) {
				m_velocity = { 0.0f, 0.0f, 0.0f };
				m_actionState = IDLE;
				setAnimActive(0);
			}
			if ((glm::distance(m_position, targetPos) <= 0.25f)) {
				m_velocity = { 0.0f, 0.0f, 0.0f };
				m_actionState = IDLE;
				setAnimActive(0);
			}

			if ((glm::distance(m_position, m_target->getPosition()) <= 0.5f) ||
				(glm::distance(m_position, (m_target->getPosition() + (m_target->getVelocity() * deltaTime * 22.0f))) <= 0.5f)) { // Prefiring Prediction for 22 frames
				m_velocity = { 0.0f, 0.0f, 0.0f };
				m_actionState = ATTACKING;
				if (fabs(m_position.x - m_target->getPosition().x) > fabs(m_position.y - m_target->getPosition().y)) {
					if (m_position.x < m_target->getPosition().x) {
						m_faceDirection = RIGHT;
						m_rotation = { m_rotation.x, 0.0f, m_rotation.z };
						setAnimActive(2);
					}
					else {
						m_faceDirection = LEFT;
						m_rotation = { m_rotation.x, 180.0f, m_rotation.z };
						setAnimActive(2);
					}
				}
				else {
					if (m_position.y > m_target->getPosition().y) {
						m_faceDirection = DOWN;
						setAnimActive(3);
					}
					else {
						m_faceDirection = UP;
						setAnimActive(4);
					}
				}
			}

			if (m_target->m_actionState == ATTACKING) {
				m_velocity = { 0.0f, 0.0f, 0.0f };
				m_actionState = IDLE;
				setAnimActive(0);
			}
		}

		if (m_actionState == ATTACKING) {
			if (m_animIndex == 4) {
				setActionState(STUNNED);
				// Knockback
				//std::cout << "FACING " << m_faceDirection << std::endl;

				glm::vec3 knockbackForce = {
					(m_faceDirection == RIGHT) ? 1.0f : (m_faceDirection == LEFT) ? -1.0f : 0.0f,
					(m_faceDirection == UP) ? 1.0f : (m_faceDirection == DOWN) ? -1.0f : 0.0f,
					0.0f };

				//std::cout << "FORCE " << knockbackForce.x << ", " << knockbackForce.y << std::endl;

				// Apply Force
				knockbackForce.x *= 0.25f;
				knockbackForce.y *= 0.25f;

				setVelocity(glm::vec3(
					knockbackForce.x,
					knockbackForce.y,
					0.0f));

				setAcceleration(glm::vec3(
					-knockbackForce.x * 2.0f,
					-knockbackForce.y * 2.0f,
					0.0f));
			}
		};
	}
	else {
		if (m_actionState == IDLE) {
			if ((rand() % 1000) < 5) {
				m_actionState = WANDER;
				setAnimActive(1);
				m_wanderTarget = {
				m_position.x + (rand() % 3) * (((rand() % 100) <= 50) ? 1.0f : -1.0f),
				m_position.y + (rand() % 3) * (((rand() % 100) <= 50) ? 1.0f : -1.0f),
				0.0f };
			}
		}
	}

	if (m_actionState == WANDER) {
		//std::cout << m_objectID << " is wandering! Is currently at: " << m_position.x << ", " << m_position.y << " aiming for: " << m_wanderTarget.x << ", " << m_wanderTarget.y << std::endl;
		m_velocity = {
				((fabs(m_position.x - m_wanderTarget.x) <= 0.05f) ? 0.0f : (m_position.x > m_wanderTarget.x) ? -1.0f : 1.0f) * m_moveSpeed * 0.5f,
				((fabs(m_position.y - m_wanderTarget.y) <= 0.05f) ? 0.0f : (m_position.y > m_wanderTarget.y) ? -1.0f : 1.0f) * m_moveSpeed * 0.5f,
				0.0f
		};
		//std::cout << m_objectID << " velocity at: " << m_velocity.x << ", " << m_velocity.y << std::endl;
		if (glm::length(m_velocity) > 1.0f)
		{
			m_velocity = glm::normalize(m_velocity);
		}
		if (m_position.x > m_wanderTarget.x) {
			m_rotation = { m_rotation.x, 180.0f, m_rotation.z };
		}
		else {
			m_rotation = { m_rotation.x, 0.0f, m_rotation.z };
		}
		for (auto& levelMap : levelMaps) {
			float x_overlap = 0.0f;
			float y_overlap = 0.0f;
			if (levelMap.getCollidable()) {
				if ((levelMap.isSolid(m_wanderTarget, &x_overlap, &y_overlap) && (x_overlap > 0) && (y_overlap > 0)) || (levelMap.isPit(m_wanderTarget))) {
					m_velocity = { 0.0f, 0.0f, 0.0f };
					m_actionState = IDLE;
					setAnimActive(0);
					//std::cout << m_objectID << "'s wanderTarget is inaccessible!" << std::endl;
				}
			}
		}
		if ((glm::distance(m_position, m_wanderTarget) <= 0.1f)) {
			m_velocity = { 0.0f, 0.0f, 0.0f };
			m_actionState = IDLE;
			setAnimActive(0);
			//std::cout << m_objectID << " reached wanderTarget!" << std::endl;
		}
		if (m_target != nullptr) {
			if (glm::distance(m_position, m_target->getPosition()) <= 3.0f) {
				m_velocity = { 0.0f, 0.0f, 0.0f };
				m_actionState = IDLE;
				setAnimActive(0);
			}
		}
	}
}