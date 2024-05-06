#include "Text.h"

Text::Text(GameObject* parent, Font font, std::string text, float fontSize) :Component(parent) {
	m_font = font;
	m_text = text;
	m_fontSize = fontSize;
};


void Text::render(ShaderProgram* program) {
	float width = 1.0f / m_font.m_cols;
	float height = 1.0f / m_font.m_rows;

	std::vector<float> vertices = {};
	std::vector<float> textureCoordinates = {};

	float screenSize = 0.25f * m_fontSize;
	float spacing = 0.025f;

	glm::vec3 drawPosition(getPosition().x, getPosition().y, 0.0f);

	if (m_textAlign == CENTERTEXTALIGN) {
		drawPosition.x += -((m_text.length() - 1) / 2.0f * screenSize) - ((spacing * m_text.length()) / 2.0f);
	}
	else if (m_textAlign == RIGHTTEXTALIGN) {
		drawPosition.x += -((m_text.length() - 1) * screenSize) - (spacing * m_text.length()) - (0.5f * screenSize);
	}
	else if (m_textAlign == LEFTTEXTALIGN) {
		drawPosition.x += (0.5f * screenSize) + spacing;
	}

	// Text Wrapping
	std::vector<std::string> drawText = { m_text };
	if (m_wrap > 0) {
		while (drawText[drawText.size() - 1].size() > m_wrap) {
			std::string line = "";
			int p1 = 0;
			int p2 = 0;
			while ((p2 < m_wrap) && (p2 != drawText[drawText.size() - 1].size())) {
				while ((drawText[drawText.size() - 1][p2] != ' ') && (p2 < m_wrap) && (p2 != drawText[drawText.size() - 1].size())) {
					p2++;
				}
				if ((drawText[drawText.size() - 1][p2] == ' ') || (p2 == drawText[drawText.size() - 1].size())) {
					while (p1 != p2) {
						line += drawText[drawText.size() - 1][p1];
						p1++;
					}
				}
				p2++;
			}

			if (drawText[drawText.size() - 1][p1] == ' ') {
				p1++;
			}

			std::string lineNext = "";
			for (int i = p1; i < drawText[drawText.size() - 1].size(); i++) {
				lineNext += drawText[drawText.size() - 1][i];
			}
			drawText.pop_back();
			drawText.push_back(line);
			if (lineNext.size() > 0) {
				drawText.push_back(lineNext);
			}
		}
	}

	for (int i = 0; i < drawText.size(); i++) {
		for (unsigned int j = 0; j < drawText[i].size(); j++) {

			int index = (int)drawText[i][j] - 32;  // ascii value of character

			float offsetX = (screenSize + spacing) * j;
			float offsetY = -(screenSize + spacing) * i;

			float uCoord = (float)(index % m_font.m_cols) / (float)m_font.m_cols;
			float vCoord = (float)(index / m_font.m_cols) / (float)m_font.m_rows;

			vertices.insert(vertices.end(), {
				offsetX + (-0.5f * screenSize), offsetY + 0.5f * screenSize,
				offsetX + (-0.5f * screenSize), offsetY + -0.5f * screenSize,
				offsetX + (0.5f * screenSize), offsetY + 0.5f * screenSize,
				offsetX + (0.5f * screenSize), offsetY + -0.5f * screenSize,
				offsetX + (0.5f * screenSize), offsetY + 0.5f * screenSize,
				offsetX + (-0.5f * screenSize), offsetY + -0.5f * screenSize,
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
	}

	glm::mat4 model_matrix = glm::mat4(1.0f);
	model_matrix = glm::translate(model_matrix, drawPosition);

	program->set_model_matrix(model_matrix);
	program->set_color(m_color[0] / 255.0f, m_color[1] / 255.0f, m_color[2] / 255.0f, m_color[3] / 255.0f);

	glUseProgram(program->get_program_id());

	glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices.data());
	glEnableVertexAttribArray(program->get_position_attribute());
	glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, textureCoordinates.data());
	glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

	glBindTexture(GL_TEXTURE_2D, m_font.m_textureID);
	glDrawArrays(GL_TRIANGLES, 0, (int)(m_text.size() * 6));

	glDisableVertexAttribArray(program->get_position_attribute());
	glDisableVertexAttribArray(program->get_tex_coordinate_attribute());

	program->set_color(1.0f, 1.0f, 1.0f, 1.0f);
}