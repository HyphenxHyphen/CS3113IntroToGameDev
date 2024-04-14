#include "Util.h"

#define STB_IMAGE_IMPLEMENTATION
#include <SDL_image.h>
#include "stb_image.h"

GLuint Util::loadTexture(const char* filepath) {
	int width, height, number_of_components;
	unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);
	if (image == NULL) {
		std::cout << "Unable to load image at: " << filepath << std::endl;
		assert(false);
	}

	GLuint texture_id;
	glGenTextures(NUMBER_OF_TEXTURES, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	stbi_image_free(image);

	return texture_id;
}

std::vector<unsigned int> Util::loadLevelData(const char* filepath) {
	int width, height, number_of_components;
	unsigned char* map = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);
	std::vector<unsigned int> levelData = {};

	if (map == NULL) {
		std::cout << "Unable to load map levelData at: " << filepath << std::endl;
		assert(false);
	}

	//std::cout << "Reading LevelData" << std::endl;
	//std::cout << width << ", " << height << std::endl;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			//std::cout << (unsigned int)(map[(y * width + x) * STBI_rgb_alpha] == 0) << " ,";
			levelData.push_back((unsigned int)(map[(y * width + x) * STBI_rgb_alpha] == 0));
		}
		//std::cout << std::endl;
	}
	//std::cout << "Returning LevelData, length = " << levelData.size() << std::endl;

	return levelData;
}

void Util::drawText(ShaderProgram* program, std::string text, glm::vec3 position, float fontSize, int fontCols, int fontRows, GLuint fontTextureID) {
	float width = 1.0f / fontCols;
	float height = 1.0f /fontRows;

	std::vector<float> vertices;
	std::vector<float> textureCoordinates;

	float screenSize = 0.25f * fontSize;
	float spacing = 0.01f;

	position.x += -((text.length() - 1) / 2.0f * screenSize);

	for (unsigned int i = 0; i < text.size(); i++) {

		int index = (int)text[i] - 48;  // ascii value of character
		if (index == -16) {
			index = 16;
		}
		float offset = (screenSize + spacing) * i;

		float uCoord = (float)(index % fontCols) / (float)fontCols;
		float vCoord = (float)(index / fontCols) / (float)fontRows;

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
	model_matrix = glm::translate(model_matrix, position);

	program->set_model_matrix(model_matrix);
	glUseProgram(program->get_program_id());

	glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices.data());
	glEnableVertexAttribArray(program->get_position_attribute());
	glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, textureCoordinates.data());
	glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

	glBindTexture(GL_TEXTURE_2D, fontTextureID);
	glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

	glDisableVertexAttribArray(program->get_position_attribute());
	glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}