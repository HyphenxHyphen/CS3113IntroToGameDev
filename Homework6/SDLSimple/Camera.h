#pragma once
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:
	glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
	float m_scale = 1.5f;

	// Window
	const float WINDOW_WIDTH = 1280.0f;
	const float WINDOW_HEIGHT = 960.0f;

	// Projectile Matrix
	float m_orthoWidth = ((WINDOW_WIDTH / 160.0f) * 1.25f * m_scale);
	float m_orthoHeight = ((WINDOW_HEIGHT / 160.0f) * 1.25f * m_scale);

	// Viewport
	const int VIEWPORT_X = 0;
	const int VIEWPORT_Y = 0;
	const float VIEWPORT_WIDTH = WINDOW_WIDTH;
	const float VIEWPORT_HEIGHT = WINDOW_HEIGHT;

	// Methods
	glm::vec3 screenToWorldPoint(glm::vec3 coord);
};

