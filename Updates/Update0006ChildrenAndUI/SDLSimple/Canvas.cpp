#include "Canvas.h"

void Canvas::update(float deltaTime) {
	setPosition({ m_cameraPos->x, m_cameraPos->y, getPosition().z });
}