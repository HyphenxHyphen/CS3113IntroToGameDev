#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include "glm/mat4x4.hpp"
#include "ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"

#include <map>   
#include <vector>
#include "Util.h"
#include "Component.h"



class SpriteRenderer : public Component
{
private:
	unsigned int m_fps = 12;
	std::vector<std::vector<int>> m_anims; // List of Animations

	unsigned int m_animIndex = 0; // Current Anim Index
	unsigned int m_animCols = 0;
	unsigned int m_animRows = 0;
	GLuint m_textureID;

	unsigned int m_animActive;
	float m_animTime = 0.0f;
	bool m_isAlive = true;
public:
	// ————— METHODS ————— //
	SpriteRenderer(GLint textureID, unsigned int animCols, unsigned int animRows, unsigned int animIndex, std::vector<std::vector<int>> anims, unsigned int startingAnim, unsigned fps);

	void drawFromAtlas(ShaderProgram* program, GLuint textureID, int index);
	void update(float deltaTime);
	void render(ShaderProgram* program);

	// ————— GETTERS ————— //
	int const getAnimActive() const { return m_animActive; };

	// ————— SETTERS ————— //
	void const setAnimActive(int animActive) { m_animActive = animActive; m_animIndex = 0; };
};

