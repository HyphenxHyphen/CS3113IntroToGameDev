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

	m_velocity = glm::vec3(0.0f);
	m_acceleration = glm::vec3(0.0f);

	m_modelMatrix = glm::mat4(1.0f);
}

// Entity.cpp
Entity::Entity(GLint textureID, unsigned int animCols, unsigned int animRows, unsigned int animIndex, std::vector<std::vector<int>> anims, unsigned int startingAnim,
	int health, int attack, float moveSpeed, glm::vec3 collisionSize)
{
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

	//setCollisionSize(collisionSize);

	m_velocity = glm::vec3(0.0f);
	m_acceleration = glm::vec3(0.0f);

	m_modelMatrix = glm::mat4(1.0f);
}

Entity::~Entity()
{
	m_collidingWith.clear();
	// Is there a memory leak?
}

void Entity::update(float deltaTime)
{
	// Animations
	if ((m_anims.size() > 0) && ((m_actionState != STUNNED) || (m_carrier != nullptr))) {
		if (m_anims[m_animActive].size() > 0) {
			m_animTime += deltaTime;
			float frames_per_second = (float)1.0f / FRAMES_PER_SECOND;

			if (m_animTime >= frames_per_second) {
				m_animTime = 0.0f;
				m_animIndex++;

				/*
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
				*/

				if (m_animIndex >= m_anims[m_animActive].size()) {
					m_animIndex = 0;
				}
			}
		}
	}

	// Activate AI
	//if (m_aiType != NONE) { aiActivate(levelMaps, deltaTime); }

	// Initial velocity
	//m_velocity.x = m_movement.x * m_moveSpeed;

	m_velocity += m_acceleration * deltaTime;

	/*
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
	*/

	// Check Movement Collision

	/*
	if ((m_actionState != FALLING) && (m_actionState != JUMPING) && (m_actionState != STUNNED)) {
		for (auto& levelMap : levelMaps) {
			if (levelMap.getCollidable()) {
				willCollide(&levelMap, levelMap.isPit(), deltaTime);
			}
			if ((levelMap.isPit()) && (levelMap.m_elevation == m_elevation)) {
				if (levelMap.isPit(m_position)) {
					fall(-m_elevation);
				}
			}
		}
	}

	if ((m_actionState != FALLING) && (m_actionState != JUMPING) && (m_actionState != STUNNED)) {
		if (m_health <= 0) {
			diesOfCringe(allEffects, deltaTime);
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
				if (m_elevation <= 0) {
					//std::cout << m_objectID << " is over Pit " << std::endl;
					diesOfCringe(deltaTime);
				}
				else {
					//std::cout << m_objectID << " Jump land on Pit " << std::endl;
					fall(-m_elevation);
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

	// Move based on Velocity
	m_position.x += (m_velocity.x * deltaTime * m_moveSpeed);
	m_position.y += (m_velocity.y * deltaTime * m_moveSpeed);

	if (m_actionState == JUMPING) {
		m_fallTarget.x += (m_velocity.x * deltaTime * m_moveSpeed);
	}
	*/

	// Apply transformation
	m_modelMatrix = glm::mat4(1.0f);
	m_modelMatrix = glm::translate(m_modelMatrix, { getPosition().x,  getPosition().y, 0.0f });
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_modelMatrix = glm::scale(m_modelMatrix, getScale());
}

/*
void Entity::render(ShaderProgram* program) {
	glm::mat4 tempModelMatrix = m_modelMatrix;
	program->set_model_matrix(tempModelMatrix);
	if ((m_anims.size() > 0) && (m_anims[m_animActive].size() > 0)) {
		drawFromAtlas(program, m_textureID, m_anims[m_animActive][m_animIndex]);
	}
	else if ((m_animRows > 0) && (m_animCols > 0)) {
		drawFromAtlas(program, m_textureID, m_animIndex);
	}
}
*/

/*
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
*/

/*
void Entity::checkCollision(std::map<std::string, Entity*>& allEntities) {
	checkColliding(allEntities);

	for (auto& collidingWith : m_collidingWith) {
		//std::cout << m_objectID << " is colliding with " << collidingWith.second->m_objectID << std::endl;

		float x_distance = fabs(getPosition().x - collidingWith.second->getPosition().x);
		float x_overlap = fabs(x_distance - (getCollisionSize().x / 2.0f) - (collidingWith.second->getCollisionSize().x / 2.0f));

		float y_distance = fabs(getPosition().y - collidingWith.second->getPosition().y);
		float y_overlap = fabs(y_distance - (getCollisionSize().y / 2.0f) - (collidingWith.second->getCollisionSize().y / 2.0f));

		if ((x_overlap < y_overlap) && ((x_overlap > 0))) {
			setPosition(X, getPosition().x + x_overlap * ((getPosition().x > collidingWith.second->getPosition().x) ? 1.0f : -1.0f));
			m_velocity.x = 0.0f;
		}
		else if (y_overlap > 0) {
			setPosition(Y, getPosition().y + y_overlap * ((getPosition().y > collidingWith.second->getPosition().y) ? 1.0f : -1.0f));
			m_velocity.y = 0.0f;
		}
	}

	cleanCollidingWith();
}
*/

/*
void Entity::willCollide(GameMap* gameMap, bool pit, float deltaTime) {
	std::vector<glm::vec3> probes = {
	glm::vec3(getPosition().x, getPosition().y + ((getScale().y / 2) - (getCollisionSize().y / 1.1f)), getPosition().z), // Top
	glm::vec3(getPosition().x - ((getScale().x / 2) - (getCollisionSize().x / 2.0f)), getPosition().y + ((getScale().y / 2) - (getCollisionSize().y / 1.1f)), getPosition().z), // Top Left
	glm::vec3(getPosition().x + ((getScale().x / 2) - (getCollisionSize().x / 2.0f)), getPosition().y + ((getScale().y / 2) - (getCollisionSize().y / 1.1f)), getPosition().z), // Top Right

	glm::vec3(getPosition().x, getPosition().y - ((getScale().y / 2) - (getCollisionSize().y / 2.0f)), getPosition().z), // Bottom
	glm::vec3(getPosition().x - ((getScale().x / 2) - (getCollisionSize().x / 2.0f)), getPosition().y - ((getScale().y / 2) - (getCollisionSize().y / 2.0f)), getPosition().z), // Bottom Left
	glm::vec3(getPosition().x + ((getScale().x / 2) - (getCollisionSize().x / 2.0f)), getPosition().y - ((getScale().y / 2) - (getCollisionSize().y / 2.0f)), getPosition().z), // Bottom Right

	glm::vec3(getPosition().x - ((getScale().x / 2) - (getCollisionSize().x / 2.0f)), getPosition().y, getPosition().z), // Left
	glm::vec3(getPosition().x + ((getScale().x / 2) - (getCollisionSize().x / 2.0f)), getPosition().y, getPosition().z), // Right
	};

	float x_overlap = 0.0f;
	float y_overlap = 0.0f;
	bool x_move = true;
	bool y_move = true;

	for (auto& probe : probes) {
		glm::vec3 newProbePos = { probe.x + m_velocity.x * deltaTime, probe.y, probe.z };
		if ((getPosition().z == gameMap->getPosition().z) && gameMap->isSolid(newProbePos, &x_overlap, &y_overlap) && (x_overlap >= 0) && (y_overlap > 0)) {
			if ((x_overlap < y_overlap)) {
				if (x_move) {
					if (m_velocity.x * (probe.x - getPosition().x) > 0) {
						//std::cout << m_objectID << " attempting to Move into Solid Tile x" << std::endl;
						if ((m_actionState != STUNNED) || !pit)  {
							setPosition(X, getPosition().x + (m_velocity.x * deltaTime) - (x_overlap * ((m_velocity.x > 0) ? 1.0f : -1.0f)));
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
		if ((getPosition().z == gameMap->getPosition().z) && gameMap->isSolid(newProbePos, &x_overlap, &y_overlap) && (x_overlap > 0) && (y_overlap >= 0)) {
			if (y_move) {
				if (m_velocity.y * (probe.y - getPosition().y) > 0) {
					//std::cout << "Attempting to Move into Solid Tile y, x over: " << x_overlap << ", y over: " << y_overlap << std::endl;
					if ((m_actionState != STUNNED) || !pit) {
						setPosition(Y, getPosition().y + (m_velocity.y * deltaTime) - (y_overlap * ((m_velocity.y > 0) ? 1.0f : -1.0f)));
						m_velocity.y = 0;
					}
				}
				y_move = false;
			}
		}
	}
}
*/

/*
void Entity::fall(float elevationTarget) {
	//std::cout << "------ fall called ------" << m_isAlive <<  std::endl;
	m_actionState = FALLING;
	setAnimActive(1);
	m_velocity.x = 0.0f;
	m_velocity.y = 1.0f;
	m_acceleration.x = 0.0f;
	m_acceleration.y = -2.0f;
	m_fallTarget = m_position;
	m_fallTarget.y += elevationTarget;
}
*/

/*
void Entity::checkColliding(std::map<std::string, Entity*>& allEntities)
{
	for (auto& entityPair : allEntities) {
		if (entityPair.first != getObjectID()) {
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
		if (entityPair.first != getObjectID()) {
			if (!isCollidingWith(entityPair.second)) {
				m_collidingWith.erase(entityPair.first);
			}
		}
	}
}
*/

/*
bool Entity::isCollidingWith(const Entity* entity) {
	float xColDistance = fabs(getPosition().x - entity->getPosition().x) - ((getCollisionSize().x + entity->getCollisionSize().x) / 2.0f);
	float yColDistance = fabs(getPosition().y - entity->getPosition().y) - ((getCollisionSize().y + entity->getCollisionSize().y) / 2.0f);

	if ((xColDistance < 0.0f) && (yColDistance < 0.0f)) {
		return true;
	}

	return false;
}

void Entity::spawnHitBox(glm::vec3& hitBoxPosition, glm::vec3& hitBoxSize, std::vector<Entity*>* entitiesHit) {
	for (auto gameObject : *getAllGameObjects()) {
		if (Entity* entity = gameObject->getComponent<Entity>()) {
			float xColDistance = fabs(hitBoxPosition.x - entity->getPosition().x) - ((hitBoxSize.x + entity->getCollisionSize().x) / 2.0f);
			float yColDistance = fabs(hitBoxPosition.y - entity->getPosition().y) - ((hitBoxSize.y + entity->getCollisionSize().y) / 2.0f);

			if ((xColDistance < 0.0f) && (yColDistance < 0.0f) && (entity->getActionState() != JUMPING) && (entity->getActionState() != FALLING)) {
				entitiesHit->push_back(entity);
			}
		}
	}
}
*/

/*
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
*/

void Entity::jump() {
	//std::cout << "------ jump called ------" << std::endl;

	m_actionState = JUMPING;
	setAnimActive(1);
	m_velocity.y = 2.0f;
	m_acceleration.y = -3.0f;
	m_fallTarget = getPosition();
}