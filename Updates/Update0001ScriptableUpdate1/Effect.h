#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include "glm/mat4x4.hpp"
#include "ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"
#include "ScriptableScript.h"

#include <map>   
#include <vector>

class Effect : public ScriptableScript
{
public:
	// ————— STATIC VARIABLES ————— //
	static const int FRAMES_PER_SECOND = 12;

	// ————— ANIMATION ————— //
	std::vector<std::vector<int>> m_anims; // List of Animations

	unsigned int m_animIndex = 0; // Current Anim Index
	unsigned int m_animCols = 0;
	unsigned int m_animRows = 0;

	unsigned int m_animActive;
	float m_animTime = 0.0f;
	bool m_animLoop = true;
	bool m_isAlive = true;
	//float m_elevation = 1.0f;

	glm::mat4 m_modelMatrix;
	GLuint m_textureID;

	// ————— METHODS ————— //
	Effect();
	Effect(GLint textureID, unsigned int animCols, unsigned int animRows, unsigned int animIndex, std::vector<std::vector<int>> anims, unsigned int startingAnim, bool animLoop);
	~Effect();

	void drawFromAtlas(ShaderProgram* program, GLuint textureID, int index);
	void update() {};
	void update(float deltaTime);
	void render(ShaderProgram* program);

	// ————— GETTERS ————— //
	int const getAnimActive() const { return m_animActive; };


	// ————— SETTERS ————— //
	void const setAnimActive(int animActive) { m_animActive = animActive; m_animIndex = 0; };
};