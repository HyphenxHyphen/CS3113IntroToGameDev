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
#include "GameMap.h"
#include "Effect.h"
#include "Util.h"
#include "Renderable.h"

enum ActionState { MOVING, ATTACKING, IDLE, DEAD, STUNNED, FALLING, JUMPING, WANDER, CARRY };
enum FaceDirection { UP, DOWN, LEFT, RIGHT };
enum AIType { NONE, GUARD, VILLAGER, GOBLIN };

// Entity.h
class Entity : public Renderable
{
private:
	glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };
	glm::vec3 m_velocity;
	glm::vec3 m_acceleration;

public:
	// ————— OBJECT ID ————— //
	std::string m_objectID = "";

	// ————— STATIC VARIABLES ————— //
	static const int FRAMES_PER_SECOND = 12;

	// ————— ANIMATION ————— //
	std::vector<std::vector<int>> m_anims; // List of Animations

	unsigned int m_animIndex = 0; // Current Anim Index
	unsigned int m_animCols = 0;
	unsigned int m_animRows = 0;

	unsigned int m_animActive;
	float m_animTime = 0.0f;
	bool m_isAlive = true;

	// ————— AI ————— //
	ActionState m_actionState = IDLE;
	FaceDirection m_faceDirection = RIGHT;
	AIType m_aiType = NONE;
	Entity* m_target = nullptr;
	Entity* m_carrier = nullptr;

	// ————— STATS ————— //
	int m_health = 0;
	int m_attack = 0;
	//float m_elevation = 1.0f;
	glm::vec3 m_fallTarget = { 0.0f, 0.0f, 0.0f };
	bool m_evil = false;
	glm::vec3 m_wanderTarget = { 0.0f, 0.0f, 0.0f };

	// ————— TRANSFORMATIONS ————— //
	float m_moveSpeed;
	glm::vec3 m_movement;
	glm::mat4 m_modelMatrix;

	GLuint m_textureID;

	// _____ COLLISIONS _____ //
	std::map<std::string, const Entity*> m_collidingWith;
	std::vector<glm::vec3> m_collidingWithTiles;
	glm::vec3 m_collisionSize;

	// ————— METHODS ————— //
	Entity();
	Entity(std::string objectID, GLint textureID, unsigned int animCols, unsigned int animRows, unsigned int animIndex, std::vector<std::vector<int>> anims, unsigned int startingAnim, int health, int attack, float moveSpeed, float elevation, glm::vec3 collisionSize);
	~Entity();

	void drawFromAtlas(ShaderProgram* program, GLuint textureID, int index);
	void update(std::vector<GameMap>& levelMaps, std::map<std::string, Entity*>& allOther, std::map<std::string, Entity*>& allEnemiesTrue, std::map<std::string, Entity*>& allEnemies, std::vector<Effect>* allEffects, float deltaTime);
	void render(ShaderProgram* program);
	void checkCollision(std::map<std::string, Entity*>& allEntities);
	void checkCollision(GameMap* gameMap);
	void willCollide(GameMap* gameMap, bool pit, float deltaTime);
	void fall(float elevationTarget);
	void checkColliding(std::map<std::string, Entity*>& allEntities);
	void cleanCollidingWith();
	bool isCollidingWith(const Entity* entity);
	void spawnHitBox(glm::vec3& hitBoxPosition, glm::vec3& hitBoxSize, std::map<std::string, Entity*>& allEntities, std::vector<Entity*>* entitiesHit);
	void diesOfCringe(std::vector<Effect>* allEffects, float deltaTime);
	void jump();
	void aiActivate(std::vector<GameMap>& levelMaps, float deltaTime);
	void aiGuard(float deltaTime);
	void aiVillager(std::vector<GameMap>& levelMaps, float deltaTime);
	void aiGoblin(std::vector<GameMap>& levelMaps, float deltaTime);


	// ————— GETTERS ————— //
	ActionState const getActionState() const { return m_actionState; };
	FaceDirection const getFaceDirection() const { return m_faceDirection; };
	glm::vec3 const getPosition() const { return m_position; };
	glm::vec3 const getRotation() const { return m_rotation; };
	glm::vec3 const getScale() const { return m_scale; };
	int const getAnimActive() const { return m_animActive; };
	int const getHealth() const { return m_health; };
	int const getAttack() const { return m_attack; };
	glm::vec3 const getMovement() const { return m_movement; };
	glm::vec3 const getVelocity() const { return m_velocity; };
	glm::vec3 const getAcceleration() const { return m_acceleration; };
	glm::vec3 const getCollisionSize() const { return m_collisionSize; };


	// ————— SETTERS ————— //
	void const setActionState(ActionState actionState) { m_actionState = actionState; };
	void const setFaceDirection(FaceDirection faceDirection) { m_faceDirection = faceDirection; };
	void const setPosition(glm::vec3 newPosition) { m_position = newPosition; };
	void const setRotation(glm::vec3 newRotation) { m_rotation = newRotation; };
	void const setScale(glm::vec3 newScale) { m_scale = newScale; m_collisionSize = { m_collisionSize.x * newScale.x, m_collisionSize.y * newScale.y, m_collisionSize.z * newScale.z }; };
	void const setVelocity(glm::vec3 newVelocity) { m_velocity = newVelocity; };
	void const setAcceleration(glm::vec3 newAcceleration) { m_acceleration = newAcceleration; };
	void const setAnimActive(int animActive) { m_animActive = animActive; m_animIndex = 0; };
	void const setHealth(int health) { m_health = health; };
	void const takeDamage(int damage) { m_health -= damage; };
	void const setAttack(int attack) { m_attack = attack; };
	void const setMovement(glm::vec3 newMovement) { m_movement = newMovement; };
	void const setCollisionSize(glm::vec3 newCollisionSize) { m_collisionSize = newCollisionSize; };
};