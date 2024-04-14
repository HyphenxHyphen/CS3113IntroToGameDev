#include "ScreenTransitionFX.h"

ScreenTransitionFX::ScreenTransitionFX(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	// Non textured Shader
	m_program.load("shaders/vertex.glsl", "shaders/fragment.glsl");
	m_program.set_projection_matrix(projectionMatrix);
	m_program.set_view_matrix(viewMatrix);

	m_currentEffect = NONE;
	m_alpha = 1.0f;
}

void ScreenTransitionFX::drawOverlay(){
	glUseProgram(m_program.get_program_id());

	float vertices[] =
	{
		-0.5, -0.5,
		 0.5, -0.5,
		 0.5,  0.5,

		-0.5, -0.5,
		 0.5,  0.5,
		-0.5,  0.5
	};

	glVertexAttribPointer(m_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(m_program.get_position_attribute());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(m_program.get_position_attribute());
}

void ScreenTransitionFX::start(TransitionFXType fxType, float fxSpeed)
{
	m_currentEffect = fxType;
	m_fxSpeed = fxSpeed;

	switch (m_currentEffect)
	{
	case NONE:   break;
	case FADEIN: {
		m_timer = 0.0f;
		m_alpha = 1.0f;
		m_running = true;
		break;
	}
	case FADEOUT: {
		m_alpha = 0.0f;
		m_running = true;
		break;
	}
	case BLACKSCREEN: {
		m_timer = 0.0f;
		m_alpha = 1.0f;
		m_running = true;
		break;
	}
	}
}


void ScreenTransitionFX::update(float deltaTime) {
	//std::cout << "TFX UPDATE " << m_currentEffect << std::endl;


	switch (m_currentEffect)
	{
	case NONE:   break;
	case FADEIN: {
		if (m_timer < 0.5f) {
			m_timer += deltaTime * m_fxSpeed;
			break;
		}

		if (m_alpha > 0.0f) {
			m_alpha -= deltaTime * m_fxSpeed;
			break;
		}

		m_currentEffect = NONE;
		m_running = false;
		break; 
	}
	case FADEOUT:
		if (m_alpha < 1.0f) {
			m_alpha += deltaTime * m_fxSpeed;
			break;
		}

		if (m_timer < 0.5f) {
			m_timer += deltaTime * m_fxSpeed;
			break;
		}

		m_running = false;
		break;
	case BLACKSCREEN: {
		if (m_timer < 1.0f) {
			m_timer += deltaTime * m_fxSpeed;
			break;
		}
		m_running = false;
		break;
	}
	}
}

void ScreenTransitionFX::render()
{
	glm::mat4 model_matrix = glm::mat4(1.0f);

	switch (m_currentEffect)
	{
	case NONE:   break;
	case FADEIN:
	case FADEOUT:
	case BLACKSCREEN:
		// Scale the model matrix to cover the whole screen
		model_matrix = glm::scale(model_matrix, glm::vec3(15.0f, 15.0f, 0.0f));

		// Apply the model matrix to the overlay
		m_program.set_model_matrix(model_matrix);

		// Make it black (or whichever colour you want) and solid
		m_program.set_colour(0.0f, 0.0f, 0.0f, m_alpha); // current ƒ¿-value

		// Draw
		drawOverlay();
		break;
	}
}