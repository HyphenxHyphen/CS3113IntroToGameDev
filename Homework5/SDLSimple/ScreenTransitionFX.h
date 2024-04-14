#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include <math.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

enum TransitionFXType { NONE, FADEIN, FADEOUT, BLACKSCREEN };

class ScreenTransitionFX
{
private:
	ShaderProgram m_program;
	float m_alpha;
	float m_fxSpeed;
	bool m_running;
	float m_timer;

public:
	TransitionFXType m_currentEffect;

	glm::vec3 m_viewOffset;

	ScreenTransitionFX(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);

	void drawOverlay();
	void start(TransitionFXType effectType, float fxSpeed);
	void update(float deltaTime);
	void render();

	bool const isRunning() const { return m_running; };
};

