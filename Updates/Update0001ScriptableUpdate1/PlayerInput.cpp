#include "PlayerInput.h"

void PlayerInput::processInput() {
	// Hold
	const Uint8* key_state = SDL_GetKeyboardState(NULL);
	
	// Moving
	if ((m_parent->getComponent<Entity>()->getActionState() == STUNNED) && (m_parent->getComponent<Entity>()->getCarrier() != nullptr)) {
		if ((key_state[SDL_SCANCODE_A]) && (key_state[SDL_SCANCODE_D]))
		{
			// Nothing
		}
		else if (key_state[SDL_SCANCODE_A] && (m_parent->getComponent<Entity>()->getRotation().y == 0.0f))
		{
			m_parent->getComponent<Entity>()->setRotation({ m_parent->getComponent<Entity>()->getRotation().x, 180.0f, m_parent->getComponent<Entity>()->getRotation().z });
			if ((rand() % 100) < 20) {
				m_parent->getComponent<Entity>()->setCarrier(nullptr);
				m_parent->getComponent<Entity>()->jump();
			}
		}
		else if (key_state[SDL_SCANCODE_D] && (m_parent->getComponent<Entity>()->getRotation().y == 180.0f))
		{
			m_parent->getComponent<Entity>()->setRotation({ m_parent->getComponent<Entity>()->getRotation().x, 0.0f, m_parent->getComponent<Entity>()->getRotation().z });
			if ((rand() % 100) < 20) {
				m_parent->getComponent<Entity>()->getCarrier()->setActionState(MOVING);
				m_parent->getComponent<Entity>()->setCarrier(nullptr);
				m_parent->getComponent<Entity>()->jump();
			}
		}
		else
		{
			// Nothing
		}
	}
	else if ((m_parent->getComponent<Entity>()->getActionState() == MOVING) || (m_parent->getComponent<Entity>()->getActionState() == IDLE)) {
		if ((key_state[SDL_SCANCODE_W]) && (key_state[SDL_SCANCODE_S]))
		{
			m_parent->getComponent<Entity>()->setVelocity(
				glm::vec3(
					m_parent->getComponent<Entity>()->getVelocity().x,
					0.0f,
					m_parent->getComponent<Entity>()->getVelocity().z
				)
			);
		}
		else if (key_state[SDL_SCANCODE_W])
		{
			m_parent->getComponent<Entity>()->setVelocity(
				glm::vec3(
					m_parent->getComponent<Entity>()->getVelocity().x,
					1.0f,
					m_parent->getComponent<Entity>()->getVelocity().z
				)
			);
		}
		else if (key_state[SDL_SCANCODE_S])
		{
			m_parent->getComponent<Entity>()->setVelocity(
				glm::vec3(
					m_parent->getComponent<Entity>()->getVelocity().x,
					-1.0f,
					m_parent->getComponent<Entity>()->getVelocity().z
				)
			);
		}
		else
		{
			m_parent->getComponent<Entity>()->setVelocity(
				glm::vec3(
					m_parent->getComponent<Entity>()->getVelocity().x,
					0.0f,
					m_parent->getComponent<Entity>()->getVelocity().z
				)
			);
		}

		if ((key_state[SDL_SCANCODE_A]) && (key_state[SDL_SCANCODE_D]))
		{
			m_parent->getComponent<Entity>()->setVelocity(
				glm::vec3(
					0.0f,
					m_parent->getComponent<Entity>()->getVelocity().y,
					m_parent->getComponent<Entity>()->getVelocity().z
				)
			);
		}
		else if (key_state[SDL_SCANCODE_A])
		{
			m_parent->getComponent<Entity>()->setVelocity(
				glm::vec3(
					-1.0f,
					m_parent->getComponent<Entity>()->getVelocity().y,
					m_parent->getComponent<Entity>()->getVelocity().z
				)
			);
			m_parent->getComponent<Entity>()->setRotation({ m_parent->getComponent<Entity>()->getRotation().x, 180.0f, m_parent->getComponent<Entity>()->getRotation().z });
		}
		else if (key_state[SDL_SCANCODE_D])
		{
			m_parent->getComponent<Entity>()->setVelocity(
				glm::vec3(
					1.0f,
					m_parent->getComponent<Entity>()->getVelocity().y,
					m_parent->getComponent<Entity>()->getVelocity().z
				)
			);
			m_parent->getComponent<Entity>()->setRotation({ m_parent->getComponent<Entity>()->getRotation().x, 0.0f, m_parent->getComponent<Entity>()->getRotation().z });
		}
		else
		{
			m_parent->getComponent<Entity>()->setVelocity(
				glm::vec3(
					0.0f,
					m_parent->getComponent<Entity>()->getVelocity().y,
					m_parent->getComponent<Entity>()->getVelocity().z
				)
			);
		}

		// Normalization

		if (glm::length(m_parent->getComponent<Entity>()->getVelocity()) > 1.0f)
		{
			m_parent->getComponent<Entity>()->setVelocity(glm::normalize(m_parent->getComponent<Entity>()->getVelocity()));
		}

		if ((m_parent->getComponent<Entity>()->getVelocity().x != 0) || (m_parent->getComponent<Entity>()->getVelocity().y != 0)) {
			m_parent->getComponent<Entity>()->setActionState(MOVING);
		}
		else {
			m_parent->getComponent<Entity>()->setActionState(IDLE);
		}

		if (key_state[SDL_SCANCODE_SPACE]) {
			m_parent->getComponent<Entity>()->jump();
		}

		if (m_parent->getComponent<Entity>()->getActionState() == MOVING) {
			if (m_parent->getComponent<Entity>()->getAnimActive() != 1) {
				m_parent->getComponent<Entity>()->setAnimActive(1);
			}
		}
		else if (m_parent->getComponent<Entity>()->getActionState() == IDLE) {
			if (m_parent->getComponent<Entity>()->getAnimActive() != 0) {
				m_parent->getComponent<Entity>()->setAnimActive(0);
			}
		}
	}

	// Attack Overrides Movement
	if (m_parent->getComponent<Entity>()->getActionState() == MOVING) {
		if (key_state[SDL_SCANCODE_RIGHT] || key_state[SDL_SCANCODE_LEFT] || key_state[SDL_SCANCODE_DOWN] || key_state[SDL_SCANCODE_UP]) {
			m_parent->getComponent<Entity>()->setActionState(ATTACKING);
			m_parent->getComponent<Entity>()->setVelocity(
				glm::vec3(
					0.0f,
					0.0f,
					m_parent->getComponent<Entity>()->getVelocity().z
				)
			);
		}
	}

	// Attacking
	if ((m_parent->getComponent<Entity>()->getActionState() == ATTACKING) || (m_parent->getComponent<Entity>()->getActionState() == IDLE)) {
		if (key_state[SDL_SCANCODE_RIGHT]) {
			m_parent->getComponent<Entity>()->setActionState(ATTACKING);
			m_parent->getComponent<Entity>()->setFaceDirection(RIGHT);
			m_parent->getComponent<Entity>()->setRotation({ m_parent->getComponent<Entity>()->getRotation().x, 0.0f, m_parent->getComponent<Entity>()->getRotation().z });
			if (m_parent->getComponent<Entity>()->getAnimActive() != 2) {
				m_parent->getComponent<Entity>()->setAnimActive(2);
			}
		}
		else if (key_state[SDL_SCANCODE_LEFT]) {
			m_parent->getComponent<Entity>()->setActionState(ATTACKING);
			m_parent->getComponent<Entity>()->setFaceDirection(LEFT);
			m_parent->getComponent<Entity>()->setRotation({ m_parent->getComponent<Entity>()->getRotation().x, 180.0f, m_parent->getComponent<Entity>()->getRotation().z });
			if (m_parent->getComponent<Entity>()->getAnimActive() != 2) {
				m_parent->getComponent<Entity>()->setAnimActive(2);
			}
		}
		else if (key_state[SDL_SCANCODE_DOWN]) {
			m_parent->getComponent<Entity>()->setActionState(ATTACKING);
			m_parent->getComponent<Entity>()->setFaceDirection(DOWN);
			if (m_parent->getComponent<Entity>()->getAnimActive() != 3) {
				m_parent->getComponent<Entity>()->setAnimActive(3);
			}
		}
		else if (key_state[SDL_SCANCODE_UP]) {
			m_parent->getComponent<Entity>()->setActionState(ATTACKING);
			m_parent->getComponent<Entity>()->setFaceDirection(UP);
			if (m_parent->getComponent<Entity>()->getAnimActive() != 4) {
				m_parent->getComponent<Entity>()->setAnimActive(4);
			}
		}
		else {
			m_parent->getComponent<Entity>()->setActionState(IDLE);
			if (m_parent->getComponent<Entity>()->getAnimActive() != 0) {
				m_parent->getComponent<Entity>()->setAnimActive(0);
			}
		}
	}
}