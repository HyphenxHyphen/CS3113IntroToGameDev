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
#include "ScriptableScript.h"

enum ActionState { MOVING, ATTACKING, IDLE, DEAD, STUNNED, FALLING, JUMPING, WANDER, CARRY };
enum FaceDirection { UP, DOWN, LEFT, RIGHT };
enum AIType { NONE, GUARD, VILLAGER, GOBLIN };

// Entity.h
class Entity : public ScriptableScript
{
private:
	glm::vec3 m_velocity;
	glm::vec3 m_acceleration;

	// ————— STATIC VARIABLES ————— //
	static const int FRAMES_PER_SECOND = 12;

	// ————— AI ————— //
	ActionState m_actionState = IDLE;
	FaceDirection m_faceDirection = RIGHT;
	AIType m_aiType = NONE;
	Entity* m_carrier = nullptr;

	// ————— STATS ————— //
	int m_health = 0;
	int m_attack = 0;
	glm::vec3 m_fallTarget = { 0.0f, 0.0f, 0.0f };
	bool m_evil = false;
	glm::vec3 m_wanderTarget = { 0.0f, 0.0f, 0.0f };

	// ————— TRANSFORMATIONS ————— //
	float m_moveSpeed;
	glm::vec3 m_movement;
	glm::mat4 m_modelMatrix;

public:
	// ————— ANIMATION ————— //
	std::vector<std::vector<int>> m_anims; // List of Animations

	unsigned int m_animIndex = 0; // Current Anim Index
	unsigned int m_animCols = 0;
	unsigned int m_animRows = 0;
	GLuint m_textureID;

	unsigned int m_animActive;
	float m_animTime = 0.0f;
	bool m_isAlive = true;

	// _____ COLLISIONS _____ //
	std::map<std::string, const Entity*> m_collidingWith;
	std::vector<glm::vec3> m_collidingWithTiles;

	// ————— METHODS ————— //
	Entity();
	Entity(GLint textureID, unsigned int animCols, unsigned int animRows, unsigned int animIndex, std::vector<std::vector<int>> anims, unsigned int startingAnim, int health, int attack, float moveSpeed, glm::vec3 collisionSize);
	~Entity();

	void drawFromAtlas(ShaderProgram* program, GLuint textureID, int index);
	void update(float deltaTime);
	void render(ShaderProgram* program);
	void checkCollision(std::map<std::string, Entity*>& allEntities);
	void willCollide(GameMap* gameMap, bool pit, float deltaTime);
	//void fall(float elevationTarget);
	void checkColliding(std::map<std::string, Entity*>& allEntities);
	void cleanCollidingWith();
	bool isCollidingWith(const Entity* entity);
	void spawnHitBox(glm::vec3& hitBoxPosition, glm::vec3& hitBoxSize, std::vector<Entity*>* entitiesHit);
	//void diesOfCringe(std::vector<Effect>* allEffects, float deltaTime);
	void jump();
	//void aiActivate(std::vector<GameMap>& levelMaps, float deltaTime);
	//void aiGuard(float deltaTime);
	//void aiVillager(std::vector<GameMap>& levelMaps, float deltaTime);
	//void aiGoblin(std::vector<GameMap>& levelMaps, float deltaTime);


	// ————— GETTERS ————— //
	ActionState const getActionState() const { return m_actionState; };
	FaceDirection const getFaceDirection() const { return m_faceDirection; };
	int const getAnimActive() const { return m_animActive; };
	int const getHealth() const { return m_health; };
	int const getAttack() const { return m_attack; };
	int const getMoveSpeed() const { return m_moveSpeed; };
	bool const getAlive() const { return m_isAlive; };
	glm::vec3 const getMovement() const { return m_movement; };
	glm::vec3 const getVelocity() const { return m_velocity; };
	glm::vec3 const getAcceleration() const { return m_acceleration; };
	glm::vec3 const getFallTarget() const { return m_fallTarget; };
	Entity* const getCarrier() const { return m_carrier; };


	// ————— SETTERS ————— //
	void const setActionState(ActionState actionState) { m_actionState = actionState; };
	void const setFaceDirection(FaceDirection faceDirection) { m_faceDirection = faceDirection; };
	void const setVelocity(glm::vec3 newVelocity) { m_velocity = newVelocity; };
	void const setAcceleration(glm::vec3 newAcceleration) { m_acceleration = newAcceleration; };
	void const setFallTarget(glm::vec3 newFallTarget) { m_fallTarget = newFallTarget; };
	void const setAnimActive(int animActive) { m_animActive = animActive; m_animIndex = 0; };
	void const setHealth(int health) { m_health = health; };
	void const takeDamage(int damage) { m_health -= damage; };
	void const setAttack(int attack) { m_attack = attack; };
	void const setMoveSpeed(int moveSpeed) { m_moveSpeed = moveSpeed; };
	void const setAlive(bool alive) { m_isAlive = alive; };
	void const setMovement(glm::vec3 newMovement) { m_movement = newMovement; };
	void const setCarrier(Entity* carrier) { m_carrier = carrier; };
};