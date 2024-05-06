#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include "glm/mat4x4.hpp"
#include "ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"

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
	GLuint m_textureID = 0;

	unsigned int m_animActive = 0;
	float m_animTime = 0.0f;
	bool m_animLoop = true;
	bool m_animPlay = true;
	bool m_animChangeFlag = false;

public:
	SpriteRenderer(GameObject* parent) :Component(parent) {};
	SpriteRenderer(GameObject* parent, GLint textureID, unsigned int animCols, unsigned int animRows, unsigned int animIndex, std::vector<std::vector<int>> anims, unsigned int startingAnim, unsigned fps);
	SpriteRenderer(GameObject* parent, SpriteRenderer* original);

	void drawFromAtlas(ShaderProgram* program, GLuint textureID, int index);
	void update(float deltaTime);
	void render(ShaderProgram* program);

	// ————— GETTERS ————— //
	int const getAnimActive() const { return m_animActive; };
	int const getAnimIndex() const { return m_animIndex; };
	bool const getAnimLoop() const { return m_animLoop; };
	bool const getAnimPlay() const { return m_animPlay; };
	bool const getAnimChangeFlag() const { return m_animChangeFlag; };

	// ————— SETTERS ————— //
	void const setAnimActive(int animActive) { m_animActive = animActive; m_animIndex = 0; m_animChangeFlag = true; };
	void const setAnimLoop(bool animLoop) { m_animLoop = animLoop; };
	void const setAnimPlay(bool animPlay) { m_animPlay = animPlay; };
	void const setTexture(GLuint textureID, unsigned int animCols, unsigned int animRows, unsigned int animIndex, std::vector<std::vector<int>> anims, unsigned int startingAnim, unsigned fps);
	void const setTexture(SpriteRenderer* original);
};

