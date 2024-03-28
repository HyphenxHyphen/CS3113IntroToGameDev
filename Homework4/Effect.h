#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include "glm/mat4x4.hpp"
#include "ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Renderable.h"

#include <map>   
#include <vector>

class Effect : public Renderable
{
private:
	glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };

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
	Effect(float elevation, GLint textureID, unsigned int animCols, unsigned int animRows, unsigned int animIndex, std::vector<std::vector<int>> anims, unsigned int startingAnim, bool animLoop);
	~Effect();

	void drawFromAtlas(ShaderProgram* program, GLuint textureID, int index);
	void update(float deltaTime);
	void render(ShaderProgram* program);

	// ————— GETTERS ————— //
	glm::vec3 const getPosition() const { return m_position; };
	glm::vec3 const getRotation() const { return m_rotation; };
	glm::vec3 const getScale() const { return m_scale; };
	int const getAnimActive() const { return m_animActive; };


	// ————— SETTERS ————— //
	void const setPosition(glm::vec3 newPosition) { m_position = newPosition; };
	void const setRotation(glm::vec3 newRotation) { m_rotation = newRotation; };
	void const setScale(glm::vec3 newScale) { m_scale = newScale; };
	void const setAnimActive(int animActive) { m_animActive = animActive; m_animIndex = 0; };
};