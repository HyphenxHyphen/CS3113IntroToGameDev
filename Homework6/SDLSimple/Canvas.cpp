#include "Canvas.h"

void Canvas::update(float deltaTime) {
	if (m_origin != nullptr) {
		setPosition({ m_origin->x, m_origin->y, getPosition().z });
	}
}