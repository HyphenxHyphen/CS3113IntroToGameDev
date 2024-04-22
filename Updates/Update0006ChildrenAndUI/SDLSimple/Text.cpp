#include "Text.h"

Text::Text(GameObject* parent, GameObject* canvas, Font font, std::string text, float fontSize) :Component(parent) {
	m_canvas = canvas;
	m_font = font;
	m_text = text;
	m_fontSize = fontSize;
};


void Text::render(ShaderProgram* program) {
	float width = 1.0f / m_font.m_cols;
	float height = 1.0f / m_font.m_rows;

	std::vector<float> vertices;
	std::vector<float> textureCoordinates;

	float screenSize = 0.25f * m_fontSize;
	float spacing = 0.01f;

	glm::vec3 drawPosition(getPosition().x, getPosition().y, 0.0f);

	if (m_textAlign == CENTERTEXTALIGN) {
		drawPosition.x += -((m_text.length() - 1) / 2.0f * screenSize);
	}
	else if (m_textAlign == RIGHTTEXTALIGN) {
		drawPosition.x += -((m_text.length() - 1) * screenSize);
	}
	for (unsigned int i = 0; i < m_text.size(); i++) {

		int index = (int)m_text[i] - 48;  // ascii value of character
		if (index == -16) {
			index = 16;
		}
		float offset = (screenSize + spacing) * i;

		float uCoord = (float)(index % m_font.m_cols) / (float)m_font.m_cols;
		float vCoord = (float)(index / m_font.m_cols) / (float)m_font.m_rows;

		vertices.insert(vertices.end(), {
			offset + (-0.5f * screenSize), 0.5f * screenSize,
			offset + (-0.5f * screenSize), -0.5f * screenSize,
			offset + (0.5f * screenSize), 0.5f * screenSize,
			offset + (0.5f * screenSize), -0.5f * screenSize,
			offset + (0.5f * screenSize), 0.5f * screenSize,
			offset + (-0.5f * screenSize), -0.5f * screenSize,
			});

		textureCoordinates.insert(textureCoordinates.end(), {
			uCoord, vCoord,
			uCoord, vCoord + height,
			uCoord + width, vCoord,
			uCoord + width, vCoord + height,
			uCoord + width, vCoord,
			uCoord, vCoord + height,
			});
	}

	glm::mat4 model_matrix = glm::mat4(1.0f);
	model_matrix = glm::translate(model_matrix, drawPosition);

	program->set_model_matrix(model_matrix);
	glUseProgram(program->get_program_id());

	glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices.data());
	glEnableVertexAttribArray(program->get_position_attribute());
	glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, textureCoordinates.data());
	glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

	glBindTexture(GL_TEXTURE_2D, m_font.m_textureID);
	glDrawArrays(GL_TRIANGLES, 0, (int)(m_text.size() * 6));

	glDisableVertexAttribArray(program->get_position_attribute());
	glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}