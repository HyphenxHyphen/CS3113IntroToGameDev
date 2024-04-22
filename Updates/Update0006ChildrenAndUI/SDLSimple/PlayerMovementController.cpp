#include "PlayerMovementController.h"
#include "EntityController.h"

void PlayerMovementController::processInput() {
	// Hold
	const Uint8* key_state = SDL_GetKeyboardState(NULL);

	EntityController* playerEC = getComponent<EntityController>();

	// Moving
	if ((playerEC->getActionState() == MOVING) || (playerEC->getActionState() == IDLE)) {
		if ((key_state[SDL_SCANCODE_W]) && (key_state[SDL_SCANCODE_S])) {
			getComponent<Rigidbody2D>()->setVelocity(Y, 0.0f);
		}
		else if (key_state[SDL_SCANCODE_W]) {
			getComponent<Rigidbody2D>()->setVelocity(Y, 1.0f);
		}
		else if (key_state[SDL_SCANCODE_S]) {
			getComponent<Rigidbody2D>()->setVelocity(Y, -1.0f);
		}
		else {
			getComponent<Rigidbody2D>()->setVelocity(Y, 0.0f);
		}

		if ((key_state[SDL_SCANCODE_A]) && (key_state[SDL_SCANCODE_D])) {
			getComponent<Rigidbody2D>()->setVelocity(X, 0.0f);
		}
		else if (key_state[SDL_SCANCODE_A]) {
			getComponent<Rigidbody2D>()->setVelocity(X, -1.0f);
			setRotation(Y, 180.0f);
		}
		else if (key_state[SDL_SCANCODE_D])
		{
			getComponent<Rigidbody2D>()->setVelocity(X, 1.0f);
			setRotation(Y, 0.0f);
		}
		else
		{
			getComponent<Rigidbody2D>()->setVelocity(X, 0.0f);
		}

		// Normalization
		if (glm::length(getComponent<Rigidbody2D>()->getVelocity()) > 1.0f)
		{
			getComponent<Rigidbody2D>()->setVelocity(glm::normalize(getComponent<Rigidbody2D>()->getVelocity()));
		}

		// Apply Movespeed
		getComponent<Rigidbody2D>()->setVelocity(X, getComponent<Rigidbody2D>()->getVelocity().x * playerEC->getMoveSpeed());
		getComponent<Rigidbody2D>()->setVelocity(Y, getComponent<Rigidbody2D>()->getVelocity().y * playerEC->getMoveSpeed());

		// Animation
		if ((getComponent<Rigidbody2D>()->getVelocity().x != 0) || (getComponent<Rigidbody2D>()->getVelocity().y != 0)) {
			playerEC->setActionState(MOVING);
		}
		else {
			playerEC->setActionState(IDLE);
		}

		if (key_state[SDL_SCANCODE_SPACE]) {
			playerEC->jump();
		}

		if (playerEC->getActionState() == MOVING) {
			if (getComponent<SpriteRenderer>()->getAnimActive() != 1) {
				getComponent<SpriteRenderer>()->setAnimActive(1);
			}
		}
		else if (playerEC->getActionState() == IDLE) {
			if (getComponent<SpriteRenderer>()->getAnimActive() != 0) {
				getComponent<SpriteRenderer>()->setAnimActive(0);
			}
		}
	}

	// Attack Overrides Movement
	if (playerEC->getActionState() == MOVING) {
		if (key_state[SDL_SCANCODE_RIGHT] || key_state[SDL_SCANCODE_LEFT] || key_state[SDL_SCANCODE_DOWN] || key_state[SDL_SCANCODE_UP]) {
			playerEC->setActionState(ATTACKING);
			getComponent<Rigidbody2D>()->setVelocity(X, 0.0f);
			getComponent<Rigidbody2D>()->setVelocity(Y, 0.0f);
		}
	}

	// Attacking
	if ((playerEC->getActionState() == ATTACKING) || (playerEC->getActionState() == IDLE)) {
		if (key_state[SDL_SCANCODE_RIGHT]) {
			playerEC->setActionState(ATTACKING);
			playerEC->setFaceDirection(RIGHT);
			setRotation(Y, 0.0f);
			if (getComponent<SpriteRenderer>()->getAnimActive() != 2) {
				getComponent<SpriteRenderer>()->setAnimActive(2);
			}
		}
		else if (key_state[SDL_SCANCODE_LEFT]) {
			playerEC->setActionState(ATTACKING);
			playerEC->setFaceDirection(LEFT);
			getComponent<SpriteRenderer>()->setRotation({ getComponent<SpriteRenderer>()->getRotation().x, 180.0f, getComponent<SpriteRenderer>()->getRotation().z });
			if (getComponent<SpriteRenderer>()->getAnimActive() != 2) {
				getComponent<SpriteRenderer>()->setAnimActive(2);
			}
		}
		else if (key_state[SDL_SCANCODE_DOWN]) {
			playerEC->setActionState(ATTACKING);
			playerEC->setFaceDirection(DOWN);
			if (getComponent<SpriteRenderer>()->getAnimActive() != 3) {
				getComponent<SpriteRenderer>()->setAnimActive(3);
			}
		}
		else if (key_state[SDL_SCANCODE_UP]) {
			playerEC->setActionState(ATTACKING);
			playerEC->setFaceDirection(UP);
			if (getComponent<SpriteRenderer>()->getAnimActive() != 4) {
				getComponent<SpriteRenderer>()->setAnimActive(4);
			}
		}
		else {
			playerEC->setActionState(IDLE);
			if (getComponent<SpriteRenderer>()->getAnimActive() != 0) {
				getComponent<SpriteRenderer>()->setAnimActive(0);
			}
		}
	}
}