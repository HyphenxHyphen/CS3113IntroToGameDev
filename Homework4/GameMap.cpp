/**
* Author: John Li
* Assignment: Rise of the AI
* Date due: 2024-03-30, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "GameMap.h"

GameMap::GameMap() {
	m_collidable = false;
	m_pit = false;

	m_width = 0;
	m_height = 0;

	m_animIndex = 0;
	m_anims = {};
	m_animActive = 0;
	m_animTime = 0.0f;
	m_animLCM = 0;

	m_levelDataRaw = {};
	m_levelDataProcessed = {};
	m_levelDataAnimated = {};
	m_tileTextureID = {};

	m_tileTextureSize = 0.0f;
	m_tileTextureCols = 0;
	m_tileTextureRows = 0;
}

GameMap::GameMap(bool collidable, bool pit, float elevation, int width, int height, unsigned int animIndex, std::map<unsigned int, std::vector<int>> anims, unsigned int startingAnim, std::vector<unsigned int> levelDataRaw, GLuint tileTextureID, float tile_size, int tileTextureCols, int tileTextureRows, std::map<std::string, int> tileTextureRules) {
	m_collidable = collidable;
	m_pit = pit;
	m_elevation = elevation;

	m_width = width;
	m_height = height;

	m_animIndex = animIndex;
	m_anims = anims;
	m_animActive = startingAnim;
	m_animTime = 0.0f;
	m_animLCM = 1;
	for (auto& anim : m_anims) {
		m_animLCM *= anim.second.size(); // Lazy LCM
	}

	m_tileTextureRules = tileTextureRules;

	m_levelDataRaw = levelDataRaw;
	m_levelDataProcessed = processLevelData(levelDataRaw);
	m_levelDataAnimated = m_levelDataProcessed;
	m_tileTextureID = tileTextureID;

	m_tileTextureSize = tile_size;
	m_tileTextureCols = tileTextureCols;
	m_tileTextureRows = tileTextureRows;

	build();
}

std::vector<unsigned int> GameMap::processLevelData(std::vector<unsigned int> levelDataRaw) {
	std::vector<unsigned int> levelDataProcessed = levelDataRaw;
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			//levelDataRaw[y_coord * m_width + x_coord]
			// Get the current tile
			unsigned int tile = levelDataRaw[y * m_width + x];
			if (tile != 0) {
				// Tiling

				int textureIndex = m_tileTextureRules["[]"];
				if ((((y + 1) < m_height) && ((y - 1) >= 0) && ((x + 1) < m_width) && ((x - 1) >= 0)) &&
					(levelDataRaw[(y + 1) * m_width + x] && levelDataRaw[(y - 1) * m_width + x] && levelDataRaw[y * m_width + x + 1] && levelDataRaw[y * m_width + x - 1])) {
					textureIndex = m_tileTextureRules["MC"];
				}
				else if ((((y + 1) < m_height) && ((y - 1) >= 0) && ((x + 1) < m_width)) &&
					(levelDataRaw[(y + 1) * m_width + x] && levelDataRaw[(y - 1) * m_width + x] && levelDataRaw[y * m_width + x + 1])) {
					textureIndex = m_tileTextureRules["ML"];
				}
				else if ((((y + 1) < m_height) && ((y - 1) >= 0) && ((x - 1) >= 0)) &&
					(levelDataRaw[(y + 1) * m_width + x] && levelDataRaw[(y - 1) * m_width + x] && levelDataRaw[y * m_width + x - 1])) {
					textureIndex = m_tileTextureRules["MR"];
				}
				else if ((((y + 1) < m_height) && ((x + 1) < m_width) && ((x - 1) >= 0)) &&
					(levelDataRaw[(y + 1) * m_width + x] && levelDataRaw[y * m_width + x + 1] && levelDataRaw[y * m_width + x - 1])) {
					textureIndex = m_tileTextureRules["TC"];
				}
				else if ((((y + 1) < m_height) && ((x + 1) < m_width)) &&
					(levelDataRaw[(y + 1) * m_width + x] && levelDataRaw[y * m_width + x + 1])) {
					textureIndex = m_tileTextureRules["TL"];
				}
				else if ((((y + 1) < m_height) && ((x - 1) >= 0)) &&
					(levelDataRaw[(y + 1) * m_width + x] && levelDataRaw[y * m_width + x - 1])) {
					textureIndex = m_tileTextureRules["TR"];
				}
				else if ((((y - 1) >= 0) && ((x + 1) < m_width) && ((x - 1) >= 0)) &&
					(levelDataRaw[(y - 1) * m_width + x] && levelDataRaw[y * m_width + x + 1] && levelDataRaw[y * m_width + x - 1])) {
					textureIndex = m_tileTextureRules["BC"];
				}
				else if ((((y - 1) >= 0) && ((x + 1) < m_width)) &&
					(levelDataRaw[(y - 1) * m_width + x] && levelDataRaw[y * m_width + x + 1])) {
					textureIndex = m_tileTextureRules["BL"];
				}
				else if ((((y - 1) >= 0) && ((x - 1) >= 0)) &&
					(levelDataRaw[(y - 1) * m_width + x] && levelDataRaw[y * m_width + x - 1])) {
					textureIndex = m_tileTextureRules["BR"];
				}
				else if ((((y + 1) < m_height) && ((y - 1) >= 0)) &&
					(levelDataRaw[(y + 1) * m_width + x] && levelDataRaw[(y - 1) * m_width + x])) {
					textureIndex = m_tileTextureRules["VM"];
				}
				else if (((y + 1) < m_height) &&
					(levelDataRaw[(y + 1) * m_width + x])) {
					textureIndex = m_tileTextureRules["VT"];
				}
				else if (((y - 1) >= 0) &&
					(levelDataRaw[(y - 1) * m_width + x])) {
					textureIndex = m_tileTextureRules["VB"];
				}
				else if (((((x + 1) < m_width) && ((x - 1) >= 0))) &&
					(levelDataRaw[y * m_width + x + 1] && levelDataRaw[y * m_width + x - 1])) {
					textureIndex = m_tileTextureRules["HC"];
				}
				else if (((x + 1) < m_width) &&
					(levelDataRaw[y * m_width + x + 1])) {
					textureIndex = m_tileTextureRules["HL"];
				}
				else if (((x - 1) >= 0) &&
					(levelDataRaw[y * m_width + x - 1])) {
					textureIndex = m_tileTextureRules["HR"];
				}
				else {
					textureIndex = m_tileTextureRules["[]"];
				}

				levelDataProcessed[y * m_width + x] = textureIndex;
			}
		}
	}
	return levelDataProcessed;
}


void GameMap::build()
{
	m_vertices.clear();
	m_textureCoordinates.clear();

	// Since this is a 2D map, we need a nested for-loop
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			// Get the current tile
			unsigned int tile = m_levelDataProcessed[y * m_width + x];
			if (m_levelDataRaw[y * m_width + x] != 0) {
				if (m_anims.count(tile)) {
					tile = m_anims[tile][m_animIndex % m_anims[tile].size()];
				}
				float uCoord = (float)(tile % m_tileTextureCols) / (float)m_tileTextureCols;
				float vCoord = (float)(tile / m_tileTextureCols) / (float)m_tileTextureRows;

				float tileWidth = 1.0f / (float)m_tileTextureCols;
				float tileHeight = 1.0f / (float)m_tileTextureRows;

				float x_offset = -(m_tileTextureSize / 2); // From center of tile
				float y_offset = (m_tileTextureSize / 2); // From center of tile

				m_vertices.insert(m_vertices.end(), {
					x_offset + (m_tileTextureSize * x),  y_offset + -m_tileTextureSize * y,
					x_offset + (m_tileTextureSize * x),  y_offset + (-m_tileTextureSize * y) - m_tileTextureSize,
					x_offset + (m_tileTextureSize * x) + m_tileTextureSize, y_offset + (-m_tileTextureSize * y) - m_tileTextureSize,
					x_offset + (m_tileTextureSize * x), y_offset + -m_tileTextureSize * y,
					x_offset + (m_tileTextureSize * x) + m_tileTextureSize, y_offset + (-m_tileTextureSize * y) - m_tileTextureSize,
					x_offset + (m_tileTextureSize * x) + m_tileTextureSize, y_offset + -m_tileTextureSize * y
					});

				m_textureCoordinates.insert(m_textureCoordinates.end(), {
					uCoord, vCoord,
					uCoord, vCoord + (tileHeight),
					uCoord + tileWidth, vCoord + (tileHeight),
					uCoord, vCoord,
					uCoord + tileWidth, vCoord + (tileHeight),
					uCoord + tileWidth, vCoord
					});
			}
		}

		m_bounds["U"] = 0 + (m_tileTextureSize / 2);
		m_bounds["D"] = -(m_tileTextureSize * m_height) + (m_tileTextureSize / 2);
		m_bounds["L"] = 0 - (m_tileTextureSize / 2);
		m_bounds["R"] = (m_tileTextureSize * m_width) - (m_tileTextureSize / 2);
	}
}

void GameMap::render(ShaderProgram* program)
{
	glm::mat4 model_matrix = glm::mat4(1.0f);
	program->set_model_matrix(model_matrix);
	//glUseProgram(program->get_program_id());

	glBindTexture(GL_TEXTURE_2D, m_tileTextureID);

	glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, m_vertices.data());
	glEnableVertexAttribArray(program->get_position_attribute());

	glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, m_textureCoordinates.data());
	glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

	glDrawArrays(GL_TRIANGLES, 0, (int)m_vertices.size() / 2);

	glDisableVertexAttribArray(program->get_position_attribute());
	glDisableVertexAttribArray(program->get_position_attribute());
}

bool GameMap::isSolid(glm::vec3 position, float* x_overlap, float* y_overlap)
{
	// The penetration between the map and the object
	// The reason why these are pointers is because we want to reassign values
	// to them in case that we are colliding. That way the object that originally
	// passed them as values will keep track of these distances
	// inb4: we're passing by reference

	// If we are out of bounds, it is not solid
	if (position.y > m_bounds["U"] || position.y < m_bounds["D"]) return false;
	if (position.x < m_bounds["L"] || position.x > m_bounds["R"])  return false;

	//int tileX = floor((position.x + (m_tileTextureSize / 2)) / m_tileTextureSize);
	int tileX = floor((round(position.x) + (m_tileTextureSize / 2)) / m_tileTextureSize);
	int tileY = -(ceil(position.y - (m_tileTextureSize / 2))) / m_tileTextureSize; // Our array counts up as Y goes down.


	//std::cout << "Probe at " << position.x << ", " << position.y << std::endl;
	//std::cout << "Checking Tile at " << tileX << ", " << tileY << std::endl;

	// If the tile index is negative or greater than the dimensions, it is not solid
	if (tileX < 0 || tileX >= m_width)  return false;
	if (tileY < 0 || tileY >= m_height) return false;

	// If the tile index is 0 i.e. an open space, it is not solid
	if (m_levelDataRaw[tileY * m_width + tileX] == 0) return false;

	// And because we likely have some overlap, we adjust for that
	*x_overlap = (m_tileTextureSize / 2) - fabs(position.x - (tileX * m_tileTextureSize));
	*y_overlap = (m_tileTextureSize / 2) - fabs(position.y + (tileY * m_tileTextureSize));

	return true;
}

bool GameMap::isPit(glm::vec3 position)
{
	// The penetration between the map and the object
	// The reason why these are pointers is because we want to reassign values
	// to them in case that we are colliding. That way the object that originally
	// passed them as values will keep track of these distances
	// inb4: we're passing by reference

	// If we are out of bounds, it is not solid
	if (position.y > m_bounds["U"] || position.y < m_bounds["D"]) return true;
	if (position.x < m_bounds["L"] || position.x > m_bounds["R"])  return true;

	//int tileX = floor((position.x + (m_tileTextureSize / 2)) / m_tileTextureSize);
	int tileX = floor((round(position.x) + (m_tileTextureSize / 2)) / m_tileTextureSize);
	int tileY = -(ceil(position.y - (m_tileTextureSize / 2))) / m_tileTextureSize; // Our array counts up as Y goes down.


	//std::cout << "Probe at " << position.x << ", " << position.y << std::endl;
	//std::cout << "Checking Tile at " << tileX << ", " << tileY << std::endl;

	// If the tile index is negative or greater than the dimensions, it is not solid
	if (tileX < 0 || tileX >= m_width)  return true;
	if (tileY < 0 || tileY >= m_height) return true;

	// If the tile index is 0 i.e. an open space, it is not solid
	if (m_levelDataRaw[tileY * m_width + tileX] == 0) return false;

	return true;
}

void GameMap::getTile(glm::vec3 position, glm::vec3* tilePosition) {
	if (position.y > m_bounds["U"] || position.y < m_bounds["D"]) return;
	if (position.x < m_bounds["L"] || position.x > m_bounds["R"])  return;
	int tileX = floor((round(position.x) + (m_tileTextureSize / 2)) / m_tileTextureSize);
	int tileY = -(ceil(position.y - (m_tileTextureSize / 2))) / m_tileTextureSize; // Our array counts up as Y goes down.
	if (tileX < 0 || tileX >= m_width)  return;
	if (tileY < 0 || tileY >= m_height) return;

	tilePosition->x = tileX;
	tilePosition->y = tileY;
}

void GameMap::update(float deltaTime) {
	if (m_anims.size() > 0) {
		m_animTime += deltaTime;
		float frames_per_second = (float)1 / FRAMES_PER_SECOND;
		if (m_animTime >= frames_per_second)
		{
			m_animTime = 0.0f;
			m_animIndex++;

			if (m_animIndex >= m_animLCM)
			{
				m_animIndex = 0;
			}
		}
	}

	build();
}