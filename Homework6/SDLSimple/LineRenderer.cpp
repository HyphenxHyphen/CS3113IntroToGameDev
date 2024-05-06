#include "LineRenderer.h"

void LineRenderer::build() {
	setScale(X, m_width);
	setScale(Y, glm::distance(m_startPos, m_endPos));
	setPosition(X, (m_startPos.x + m_endPos.x) / 2.0f);
	setPosition(Y, (m_startPos.y + m_endPos.y) / 2.0f);
	setRotation(Z, 90 + glm::degrees(atan2(m_startPos.y - m_endPos.y, m_startPos.x - m_endPos.x)));
}

void LineRenderer::render(ShaderProgram* program) {
	program->set_model_matrix(getModelMatrix());

	// Step 1: Calculate the UV location of the indexed frame
	float uCoord = 1.0f;
	float vCoord = 1.0f;

	// Step 2: Calculate its UV size
	float width = 1.0f;
	float height = 1.0f;

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
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->get_position_attribute());

	glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, textureCoordinates);
	glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->get_position_attribute());
	glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}