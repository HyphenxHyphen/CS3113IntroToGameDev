/**
* Author: John Li
* Assignment: Lunar Lander
* Date due: 2024-09-03, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Entity.h"

GLuint loadTexture(const char* filepath);

Entity::Entity() {
	m_objectID = "Blank";
	m_textureID = NULL;
	m_animRows = 0;
	m_animCols = 0;
	m_animIndex = 0;
	m_anims = {};
	m_animActive = 0;
	m_animTime = 0.0f;

	m_moveSpeed = 0.0f;

	m_collisionSize = glm::vec3(0.0f);

	m_position = glm::vec3(0.0f);
	m_velocity = glm::vec3(0.0f);
	m_acceleration = glm::vec3(0.0f);

	m_modelMatrix = glm::mat4(1.0f);
}

// Entity.cpp
Entity::Entity(std::string objectID, GLint textureID, unsigned int animRows, unsigned int animCols, unsigned int animIndex, std::vector<std::vector<int>> anims, unsigned int startingAnim, glm::vec3 position, glm::vec3 movement, float moveSpeed, glm::vec3 collisionSize)
{
	m_objectID = objectID;
	m_textureID = textureID;
	m_animRows = animRows;
	m_animCols = animCols;
	m_animIndex = animIndex;
	m_anims = anims;
	m_animActive = startingAnim;
	m_animTime = 0.0f;

	m_modelMatrix = glm::mat4(1.0f);

	m_moveSpeed = moveSpeed;

	setCollisionSize(collisionSize);

	m_position = position;
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
	if (m_anims.size() > 0) {
		if (m_anims[m_animActive].size() > 0)
		{
			if (glm::length(m_movement) != 0)
			{
				m_animTime += deltaTime;
				float frames_per_second = (float)1 / SECONDS_PER_FRAME;

				if (m_animTime >= frames_per_second)
				{
					m_animTime = 0.0f;
					m_animIndex++;

					if (m_animIndex >= m_anims[m_animActive].size())
					{
						m_animIndex = 0;
					}
				}
			}
		}
	}

	// Initial velocity
	//m_velocity.x = m_movement.x * m_moveSpeed;

	// Gravity
	m_velocity += m_acceleration * deltaTime;

	// Inertia?
	if (abs(m_acceleration.x) > 0) {
		m_acceleration.x /= 2.0f;
	}

	// Bound X
	if ((m_position.x - (m_collisionSize.x / 2.0f)) <= -5.125f) {
		float x_distance = fabs(m_position.x - -5.125f);
		float x_overlap = fabs(x_distance - (m_collisionSize.x / 2.0f));

		m_position.x += x_overlap;
		if (m_acceleration.x < 0.0f) {
			m_velocity.x = 0.0f;
		}
		m_acceleration.x = 0.0f;
	}

	if ((m_position.x + (m_collisionSize.x / 2.0f)) >= 5.125f) {
		float x_distance = fabs(m_position.x - 5.125f);
		float x_overlap = fabs(x_distance - (m_collisionSize.x / 2.0f));

		m_position.x -= x_overlap;
		if (m_acceleration.x > 0.0f) {
			m_velocity.x = 0.0f;
		}
		m_acceleration.x = 0.0f;
	}

	// Apply transformation
	m_modelMatrix = glm::mat4(1.0f);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
}

void Entity::render(ShaderProgram* program) {
	float vertices[] = {
		-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,  // triangle 1
		-0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f   // triangle 2
	};
	glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->get_position_attribute());

	// Textures
	float texCoords[] = {
		0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,     // triangle 1
		0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,     // triangle 2
	};
	glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

	program->set_model_matrix(m_modelMatrix);
	if ((m_anims.size() > 0) && (m_anims[m_animActive].size() > 0)) {
		drawFromAtlas(program, m_textureID, m_anims[m_animActive][m_animIndex]);
	}
	else if ((m_animRows > 0) && (m_animCols > 0)) {
		drawFromAtlas(program, m_textureID, m_animIndex);
	}

	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->get_position_attribute());
	glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
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
	float tex_coords[] =
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

	glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
	glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->get_position_attribute());
	glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}

void Entity::checkCollision(float deltaTime, std::map<std::string, Entity*>& allEntities) {
	checkColliding(allEntities);
	
	m_position.y += m_velocity.y * deltaTime;
	m_position.x += m_velocity.x * deltaTime;

	for (auto& collidingWith : m_collidingWith) {
		float x_distance = fabs(m_position.x - collidingWith.second->m_position.x);
		float x_overlap = fabs(x_distance - (m_collisionSize.x / 2.0f) - (collidingWith.second->m_collisionSize.x / 2.0f));

		float y_distance = fabs(m_position.y - collidingWith.second->m_position.y);
		float y_overlap = fabs(y_distance - (m_collisionSize.y / 2.0f) - (collidingWith.second->m_collisionSize.y / 2.0f));

		if (x_overlap < y_overlap) {
			m_position.x += x_overlap * ((m_position.x > collidingWith.second->m_position.x) ? 1.0f : -1.0f);
			m_velocity.x = 0.0f;
		}
		else {
			m_position.y += y_overlap * ((m_position.y > collidingWith.second->m_position.y) ? 1.0f : -1.0f);
			m_velocity.y = 0.0f;
		}
	}

	checkColliding(allEntities);
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

bool Entity::isCollidingWith(Entity* entity)
{
	float xColDistance = fabs(m_position.x - entity->m_position.x) - ((m_collisionSize.x + entity->m_collisionSize.x) / 2.0f);
	float yColDistance = fabs(m_position.y - entity->m_position.y) - ((m_collisionSize.y + entity->m_collisionSize.y) / 2.0f);

	if ((xColDistance < 0.0f) && (yColDistance < 0.0f)) {
		return true;
	}
	
	return false;
}
