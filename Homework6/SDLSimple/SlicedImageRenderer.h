#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include "glm/mat4x4.hpp"
#include "ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"

//#include <map>   
//#include "Util.h"

#include "Component.h"
class SlicedImageRenderer : public Component {
private:
	float m_width = 1.0f;
	float m_height = 1.0f;

	std::vector<GLuint> m_textureIds = {};
	int m_textureInd = 0;
	std::vector<float> m_vertices = std::vector<float>(12 * 9, 0.0f);
	std::vector<float> m_textureCoordinates = std::vector<float>(12 * 9, 0.0f);

public:
	SlicedImageRenderer(GameObject* parent) :Component(parent) {};
	SlicedImageRenderer(GameObject* parent, GLint textureId, float width, float height) :Component(parent) { m_textureIds.push_back(textureId); m_width = width; m_height = height; build(); };

	void build();
	void update(float deltaTime) {};
	void render(ShaderProgram* program);

	// ————— GETTERS ————— //
	float const getWidth() const { return m_width; };
	float const getHeight() const { return m_height; };

	// ————— SETTERS ————— //
	void setWidth(float newVal) { m_width = newVal; build(); };
	void setHeight(float newVal) { m_height = newVal; build(); };
	void const addTexture(GLint textureId) { m_textureIds.push_back(textureId); };
	void const setTexture(int textureInd) { if (m_textureInd != textureInd) { m_textureInd = textureInd; build(); } };
	void const setTexture(int textureInd, GLint textureId) { if (textureInd < m_textureIds.size()) { m_textureIds[textureInd] = textureId; m_textureInd = textureInd; build(); } };
};

