#include "Button.h"

bool Button::isHovered() {
	int x, y;
	const Uint32 mouseState = SDL_GetMouseState(&x, &y);
	glm::vec3 mousePos(x, y, 0.0f);
	mousePos = m_camera->screenToWorldPoint(mousePos);

	return ((mousePos.x >= getPosition().x - (m_image->getWidth() * getScale().x / 2.0f)) && (mousePos.x <= getPosition().x + (m_image->getWidth() * getScale().x / 2.0f)) &&
		(mousePos.y >= getPosition().y - (m_image->getHeight() * getScale().y / 2.0f)) && (mousePos.y <= getPosition().y + (m_image->getHeight() * getScale().y / 2.0f)));
}