#pragma once
#include <SDL_mixer.h>
#include "Component.h"
#include "Rigidbody2D.h"
#include "SpriteRenderer.h"
#include "TileMap.h"
#include "Collider2D.h"
#include "PolygonCollider2D.h"

enum ActionState { MOVING, ATTACKING, IDLE, DEAD, STUNNED, FALLING, JUMPING, WANDER, CARRY };
enum FaceDirection { UP, DOWN, LEFT, RIGHT };

class EntityController : public Component
{
private:
	float m_moveSpeed = 1.0f;
	ActionState m_actionState = IDLE;
	FaceDirection m_faceDirection = RIGHT;
	float m_health = 0.0f;
	float m_attack = 0.0f;
	bool m_evil = false;
	bool attackHit = false;
	glm::vec3 m_fallTarget = { 0.0f, 0.0f, 0.0f };
	Rigidbody2D* rb;
	EntityController* m_carrier = nullptr;
	EntityController* m_target = nullptr;
	Mix_Chunk* m_jumpSFX = nullptr;
	Mix_Chunk* m_atkSFX = nullptr;
	Mix_Chunk* m_hitSFX = nullptr;
	Mix_Chunk* m_dieSFX = nullptr;

public:
	EntityController(GameObject* parent) :Component(parent) { rb = getComponent<Rigidbody2D>(); };

	void update(float deltaTime);

	void fall(float elevationTarget);
	void jump();
	void diesOfCringe(float deltaTime);
	void spawnHitBox(glm::vec3& hitBoxPosition, glm::vec3& hitBoxSize, std::vector<EntityController*>* entitiesHit);
	bool isOverPit();

	// ————— GETTERS ————— //
	float const getHealth() const { return m_health; };
	float const getAttack() const { return m_attack; };
	bool const isEvil() const { return m_evil; };
	ActionState const getActionState() const { return m_actionState; };
	FaceDirection const getFaceDirection() const { return m_faceDirection; };
	float const getMoveSpeed() const { return m_moveSpeed; };
	glm::vec3 getFallTarget() const { return m_fallTarget; };
	EntityController* getCarrier() const { return m_carrier; };
	EntityController* getTarget() const { return m_target; };

	// ————— SETTERS ————— //
	void const setHealth(float health) { m_health = health; };
	void const takeDamage(float damage);
	void const setAttack(float attack) { m_attack = attack; };
	void const setEvil(bool evil) { m_evil = evil; };
	void const setActionState(ActionState actionState) { m_actionState = actionState; 
	if (actionState == STUNNED) { getComponent<SpriteRenderer>()->setAnimPlay(false); }
	else { getComponent<SpriteRenderer>()->setAnimPlay(true); }};
	void const setFaceDirection(FaceDirection faceDirection) { m_faceDirection = faceDirection; };
	void const setMoveSpeed(float moveSpeed) { m_moveSpeed = moveSpeed; };
	void const setCarrier(EntityController* carrier) { m_carrier = carrier; };
	void const setTarget(EntityController* target) { m_target = target; };
	void const setJumpSFX(Mix_Chunk* jumpSFX) { m_jumpSFX = jumpSFX; };
	void const setAtkSFX(Mix_Chunk* atkSFX) { m_atkSFX = atkSFX; };
	void const setHitSFX(Mix_Chunk* hitSFX) { m_hitSFX = hitSFX; };
	void const setDieSFX(Mix_Chunk* dieSFX) { m_dieSFX = dieSFX; };
};

