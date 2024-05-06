#include "Camera.h"

glm::vec3 Camera::screenToWorldPoint(glm::vec3 coord) {
	glm::vec3 worldPoint(0.0f);
	worldPoint.x = ((coord.x / WINDOW_WIDTH) * m_orthoWidth) - (m_orthoWidth / 2.0f);
	worldPoint.y = (((WINDOW_HEIGHT - coord.y) / WINDOW_HEIGHT) * m_orthoHeight) - (m_orthoHeight / 2.0f);
	worldPoint += m_position;
	return worldPoint;
}