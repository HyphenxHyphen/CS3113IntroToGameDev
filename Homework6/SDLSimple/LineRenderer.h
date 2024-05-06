#pragma once
#include "Component.h"
class LineRenderer : public Component {
private:
	float m_width = 0.1f;

	glm::vec3 m_startPos = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_endPos = { 0.0f, 0.0f, 0.0f };

	GLuint m_textureID = 0;

public:
	LineRenderer(GameObject* parent) :Component(parent) {};
	LineRenderer(GameObject* parent, GLint textureID, glm::vec3 startPos, glm::vec3 endPos, float width) :Component(parent) { m_textureID = textureID; m_startPos = startPos; m_endPos = endPos; m_width = width; build(); };

	void build();
	void update(float deltaTime) {};
	void render(ShaderProgram* program);
};

