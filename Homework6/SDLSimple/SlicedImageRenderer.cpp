#include "SlicedImageRenderer.h"

void SlicedImageRenderer::build() {
	unsigned int vecInd = 0;
	unsigned int texInd = 0;

	float lastPosY = 0;
	for (int y = 0; y < 3; y++) {
		float lastPosX = 0;
		for (int x = 0; x < 3; x++) {
			// Get the current tile
			int tile = y * 3 + x;

			//std::cout << std::to_string(tile) << std::endl;

			float uCoord = (float)(tile % 3) / (float)3;
			float vCoord = (float)(tile / 3) / (float)3;

			float tileWidth = 1.0f / (float)3;
			float tileHeight = 1.0f / (float)3;

			float tileTextureSizeX = (1.0f / 3.0f);
			float tileTextureSizeY = (1.0f / 3.0f);
			if ((tile == 1) || (tile == 4) || (tile == 7)) {
				tileTextureSizeX = (m_width - (2.0f / 3.0f));
			}
			if ((tile == 3) || (tile == 4) || (tile == 5)) {
				tileTextureSizeY = (m_height - (2.0f / 3.0f));
			}

			//std::cout << std::to_string(tileTextureSizeX) << std::endl;

			float x_offset = -(m_width / 2); // From center of tile
			float y_offset = (m_height / 2); // From center of tile

			//std::cout << std::to_string(m_vertices[vecInd]) << std::endl;
			//std::cout << std::to_string(lastPosX) << std::endl;
			m_vertices[vecInd] = x_offset + lastPosX;
			vecInd++;
			m_vertices[vecInd] = y_offset + lastPosY;
			vecInd++;
			m_vertices[vecInd] = x_offset + lastPosX;
			vecInd++;
			m_vertices[vecInd] = y_offset + lastPosY - tileTextureSizeY;
			vecInd++;
			m_vertices[vecInd] = x_offset + lastPosX + tileTextureSizeX;
			vecInd++;
			m_vertices[vecInd] = y_offset + lastPosY - tileTextureSizeY;
			vecInd++;
			m_vertices[vecInd] = x_offset + lastPosX;
			vecInd++;
			m_vertices[vecInd] = y_offset + lastPosY;
			vecInd++;
			m_vertices[vecInd] = x_offset + lastPosX + tileTextureSizeX;
			vecInd++;
			m_vertices[vecInd] = y_offset + lastPosY - tileTextureSizeY;
			vecInd++;
			m_vertices[vecInd] = x_offset + lastPosX + tileTextureSizeX;
			vecInd++;
			m_vertices[vecInd] = y_offset + lastPosY;
			vecInd++;

			m_textureCoordinates[texInd] = uCoord;
			texInd++;
			m_textureCoordinates[texInd] = vCoord;
			texInd++;
			m_textureCoordinates[texInd] = uCoord;
			texInd++;
			m_textureCoordinates[texInd] = vCoord + (tileHeight);
			texInd++;
			m_textureCoordinates[texInd] = uCoord + tileWidth;
			texInd++;
			m_textureCoordinates[texInd] = vCoord + (tileHeight);
			texInd++;
			m_textureCoordinates[texInd] = uCoord;
			texInd++;
			m_textureCoordinates[texInd] = vCoord;
			texInd++;
			m_textureCoordinates[texInd] = uCoord + tileWidth;
			texInd++;
			m_textureCoordinates[texInd] = vCoord + (tileHeight);
			texInd++;
			m_textureCoordinates[texInd] = uCoord + tileWidth;
			texInd++;
			m_textureCoordinates[texInd] = vCoord;
			texInd++;

			lastPosX += tileTextureSizeX;

			if ((tile == 2) || (tile == 5) || (tile == 8)) {
				lastPosY += -tileTextureSizeY;
			}
		}
	}
}

void SlicedImageRenderer::render(ShaderProgram* program) {
	//glm::mat4 model_matrix = glm::mat4(1.0f);
	program->set_model_matrix(getModelMatrix());
	//glUseProgram(program->get_program_id());

	glBindTexture(GL_TEXTURE_2D, m_textureIds[((m_textureInd >= m_textureIds.size()) ? m_textureIds.size() - 1 : m_textureInd)]);

	glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, m_vertices.data());
	glEnableVertexAttribArray(program->get_position_attribute());

	glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, m_textureCoordinates.data());
	glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

	glDrawArrays(GL_TRIANGLES, 0, (int)m_vertices.size() / 2);

	glDisableVertexAttribArray(program->get_position_attribute());
	glDisableVertexAttribArray(program->get_position_attribute());
}