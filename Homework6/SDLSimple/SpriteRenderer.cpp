#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(GameObject* parent, GLint textureID, unsigned int animCols, unsigned int animRows, unsigned int animIndex, std::vector<std::vector<int>> anims, unsigned int startingAnim, unsigned fps) :Component(parent) {
	m_textureID = textureID;
	m_animCols = animCols;
	m_animRows = animRows;
	m_animIndex = animIndex;
	m_anims = anims;
	m_animActive = startingAnim;
	m_animTime = 0.0f;
	m_fps = fps;
}

SpriteRenderer::SpriteRenderer(GameObject* parent, SpriteRenderer* original) :Component(parent) {
	m_textureID = original->m_textureID;
	m_animCols = original->m_animCols;
	m_animRows = original->m_animRows;
	m_animIndex = original->m_animIndex;
	m_anims = original->m_anims;
	m_animActive = original->m_animActive;
	m_animTime = 0.0f;
	m_fps = original->m_fps;
}

void const SpriteRenderer::setTexture(GLuint textureID, unsigned int animCols, unsigned int animRows, unsigned int animIndex, std::vector<std::vector<int>> anims, unsigned int startingAnim, unsigned fps) { 
	m_textureID = textureID; 
	m_animCols = animCols;
	m_animRows = animRows;
	m_animIndex = animIndex;
	m_anims = anims;
	m_animActive = startingAnim;
	m_animTime = 0.0f;
	m_fps = fps;
};

void const SpriteRenderer::setTexture(SpriteRenderer* original) {
	m_textureID = original->m_textureID;
	m_animCols = original->m_animCols;
	m_animRows = original->m_animRows;
	m_animIndex = original->m_animIndex;
	m_anims = original->m_anims;
	m_animActive = original->m_animActive;
	m_animTime = 0.0f;
	m_fps = original->m_fps;
};


void SpriteRenderer::update(float deltaTime) {
	// Animations
	if ((m_anims.size() > 0) && (m_animPlay)) {
		if (m_anims[m_animActive].size() > 0) {
			m_animChangeFlag = false;

			m_animTime += deltaTime;
			float frames_per_second = (float)1 / m_fps;

			if (m_animTime >= frames_per_second) {
				m_animTime = 0.0f;
				m_animIndex++;
				m_animChangeFlag = true;

				if (m_animIndex >= m_anims[m_animActive].size()) {
					m_animIndex = 0;
					if (!m_animLoop) {
						m_parent->setAlive(false);
					}
				}
			}
		}
	}
}

void SpriteRenderer::render(ShaderProgram* program) {
	program->set_model_matrix(getModelMatrix());
	if ((m_anims.size() > 0) && (m_anims[m_animActive].size() > 0)) {
		drawFromAtlas(program, m_textureID, m_anims[m_animActive][m_animIndex]);
	}
	else if ((m_animRows > 0) && (m_animCols > 0)) {
		drawFromAtlas(program, m_textureID, m_animIndex);
	}
}

void SpriteRenderer::drawFromAtlas(ShaderProgram* program, GLuint textureID, int index)
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