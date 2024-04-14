#pragma once
#include <SDL.h>
#include "Component.h"
#include "Rigidbody2D.h"
#include "SpriteRenderer.h"

enum ActionState { MOVING, ATTACKING, IDLE, DEAD, STUNNED, FALLING, JUMPING, WANDER, CARRY };
enum FaceDirection { UP, DOWN, LEFT, RIGHT };

class PlayerMovementController : public Component
{
private:
	// ————— AI ————— //
	ActionState m_actionState = IDLE;
	FaceDirection m_faceDirection = RIGHT;
	float m_moveSpeed = 1.0f;
public:
	PlayerMovementController(GameObject* parent) :Component(parent) {};

	void update(float deltaTime) {};
	void processInput();
	//void jump();

	// ————— GETTERS ————— //
	ActionState const getActionState() const { return m_actionState; };
	FaceDirection const getFaceDirection() const { return m_faceDirection; };
	float const getMoveSpeed() const { return m_moveSpeed; };

	// ————— SETTERS ————— //
	void const setActionState(ActionState actionState) { m_actionState = actionState; };
	void const setFaceDirection(FaceDirection faceDirection) { m_faceDirection = faceDirection; };
	void const setMoveSpeed(float moveSpeed) { m_moveSpeed = moveSpeed; };
};

