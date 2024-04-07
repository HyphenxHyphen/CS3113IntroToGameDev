/**
* Author: John Li
* Assignment: Rise of the AI
* Date due: 2024-03-30, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Effect.h"

Effect::Effect() {
	m_textureID = NULL;
	m_animCols = 0;
	m_animRows = 0;
	m_animIndex = 0;
	m_anims = {};
	m_animActive = 0;
	m_animTime = 0.0f;
	m_modelMatrix = glm::mat4(1.0f);
}

Effect::Effect(GLint textureID, unsigned int animCols, unsigned int animRows, unsigned int animIndex, std::vector<std::vector<int>> anims, unsigned int startingAnim, bool animLoop)
{
	//std::cout << "This is an effect at " << elevation << std::endl;
	m_textureID = textureID;
	m_animCols = animCols;
	m_animRows = animRows;
	m_animIndex = animIndex;
	m_anims = anims;
	m_animActive = startingAnim;
	m_animTime = 0.0f;
	m_animLoop = animLoop;
	m_modelMatrix = glm::mat4(1.0f);
}

Effect::~Effect()
{
	// Everything should clear
}

void Effect::update(float deltaTime)
{
	if (!m_isAlive) { return; }

	// Animations
	if (m_anims.size() > 0) {
		if (m_anims[m_animActive].size() > 0) {
			m_animTime += deltaTime;
			float frames_per_second = (float)1 / FRAMES_PER_SECOND;

			if (m_animTime >= frames_per_second) {
				m_animTime = 0.0f;
				
				if (m_animIndex < m_anims[m_animActive].size()) {
					m_animIndex++;
				}

				if (m_animIndex >= m_anims[m_animActive].size()) {
					m_animIndex = 0;
					if (!m_animLoop) {
						m_isAlive = false;
					}
				}
			}
		}
	}

	// Apply transformation
	m_modelMatrix = glm::mat4(1.0f);
	m_modelMatrix = glm::translate(m_modelMatrix, getPosition());
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_modelMatrix = glm::scale(m_modelMatrix, getScale());
}

void Effect::render(ShaderProgram* program) {
	if (!m_isAlive) { return; }
	program->set_model_matrix(m_modelMatrix);
	if ((m_anims.size() > 0) && (m_anims[m_animActive].size() > 0)) {
		drawFromAtlas(program, m_textureID, m_anims[m_animActive][m_animIndex]);
	}
	else if ((m_animRows > 0) && (m_animCols > 0)) {
		drawFromAtlas(program, m_textureID, m_animIndex);
	}
}

void Effect::drawFromAtlas(ShaderProgram* program, GLuint textureID, int index)
{
	// Step 1: Calculate the UV location of the indexed frame
	float uCoord = (float)(index % m_animCols) / (float)m_animCols;
	float vCoord = (float)(index / m_animCols) / (float)m_animRows;

	// Step 2: Calculate its UV size
	float width = 1.0f / (float)m_animCols;
	float height = 1.0f / (float)m_animRows;

	// Step 3: Just as we have done before, match the texture coordinates to the vertices
	float textureCoordinates[] =
	{
		uCoord, vCoord + height, uCoord + width, vCoord + height, uCoord + width, vCoord,
		uCoord, vCoord + height, uCoord + width, vCoord, uCoord, vCoord
	};

	float vertices[] =
	{
		-0.5, -0.5, 0.5, -0.5,  0.5, 0.5,
		-0.5, -0.5, 0.5,  0.5, -0.5, 0.5
	};

	// Step 4: And render
	glBindTexture(GL_TEXTURE_2D, textureID);

	glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->get_position_attribute());

	glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, textureCoordinates);
	glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->get_position_attribute());
	glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}