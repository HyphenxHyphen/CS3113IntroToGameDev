#pragma once


#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include "glm/mat4x4.hpp"
#include "ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"

#include <map>   
#include <vector>


// Entity.h
class Entity
{
private:
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec3 m_acceleration;
public:
	// ————— OBJECT ID ————— //
	std::string m_objectID = "";

	// ————— STATIC VARIABLES ————— //
	static const int SECONDS_PER_FRAME = 4;

	// ————— ANIMATION ————— //
	std::vector<std::vector<int>> m_anims;

	unsigned int m_animIndex = 0;
	unsigned int m_animCols = 0;
	unsigned int m_animRows = 0;

	unsigned int m_animActive;
	float m_animTime = 0.0f;

	// ————— TRANSFORMATIONS ————— //
	float     m_moveSpeed;
	glm::vec3 m_movement;
	glm::mat4 m_modelMatrix;

	GLuint    m_textureID;

	// _____ COLLISIONS _____ //
	std::map<std::string, const Entity*> m_collidingWith;
	glm::vec3 m_collisionSize;

	// ————— METHODS ————— //
	Entity();
	Entity(std::string objectID, GLint texturePath, unsigned int animRows, unsigned int animCols, unsigned int animIndex, std::vector<std::vector<int>> anims, unsigned int startingAnim, glm::vec3 position, glm::vec3 movement, float moveSpeed, glm::vec3 collisionSize);
	~Entity();

	void drawFromAtlas(ShaderProgram* program, GLuint textureID, int index);
	void update(float deltaTime);
	void render(ShaderProgram* program);
	void checkCollision(float deltaTime, std::map<std::string, Entity*>& allEntities);
	void checkColliding(std::map<std::string, Entity*>& allEntities);
	bool isCollidingWith(Entity* entity);

	// ————— GETTERS ————— //
	glm::vec3 const getPosition() const { return m_position; };
	glm::vec3 const getMovement() const { return m_movement; };
	glm::vec3 const getVelocity()     const { return m_velocity; };
	glm::vec3 const getAcceleration() const { return m_acceleration; };
	glm::vec3 const getCollisionSize(glm::vec3 newCollisionSize) const { return m_collisionSize; };


	// ————— SETTERS ————— //
	void const setPosition(glm::vec3 newPosition) { m_position = newPosition; };
	void const setVelocity(glm::vec3 newVelocity) { m_velocity = newVelocity; };
	void const setAcceleration(glm::vec3 newAcceleration) { m_acceleration = newAcceleration; };
	void const setMovement(glm::vec3 newMovement) { m_movement = newMovement; };
	void const setCollisionSize(glm::vec3 newCollisionSize) { m_collisionSize = newCollisionSize; };
};