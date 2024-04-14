#include "TileMap.h"

TileMap::TileMap(GameObject* parent) :Component(parent) {
	m_collidable = false;

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

TileMap::TileMap(GameObject* parent, bool collidable, int width, int height, unsigned int animIndex, std::map<unsigned int, std::vector<int>> anims, unsigned int startingAnim, std::vector<unsigned int> levelDataRaw, GLuint tileTextureID, float tile_size, int tileTextureCols, int tileTextureRows, std::map<std::string, int> tileTextureRules) :Component(parent) {
	m_collidable = collidable;

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

	m_vertices = std::vector<float>(12 * m_levelDataRaw.size(), 0.0f);
	m_textureCoordinates = std::vector<float>(12 * m_levelDataRaw.size(), 0.0f);

	build();

	if (collidable) {
		setupColliders();
	}
}

std::vector<unsigned int> TileMap::processLevelData(std::vector<unsigned int> levelDataRaw) {
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

void TileMap::build()
{
	//m_vertices.clear();
	//m_textureCoordinates.clear();

	// Since this is a 2D map, we need a nested for-loop

	unsigned int vecInd = 0;
	unsigned int texInd = 0;
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

				m_vertices[vecInd] = x_offset + (m_tileTextureSize * x);
				vecInd++;
				m_vertices[vecInd] = y_offset + -m_tileTextureSize * y;
				vecInd++;
				m_vertices[vecInd] = x_offset + (m_tileTextureSize * x);
				vecInd++;
				m_vertices[vecInd] = y_offset + (-m_tileTextureSize * y) - m_tileTextureSize;
				vecInd++;
				m_vertices[vecInd] = x_offset + (m_tileTextureSize * x) + m_tileTextureSize;
				vecInd++;
				m_vertices[vecInd] = y_offset + (-m_tileTextureSize * y) - m_tileTextureSize;
				vecInd++;
				m_vertices[vecInd] = x_offset + (m_tileTextureSize * x);
				vecInd++;
				m_vertices[vecInd] = y_offset + -m_tileTextureSize * y;
				vecInd++;
				m_vertices[vecInd] = x_offset + (m_tileTextureSize * x) + m_tileTextureSize;
				vecInd++;
				m_vertices[vecInd] = y_offset + (-m_tileTextureSize * y) - m_tileTextureSize;
				vecInd++;
				m_vertices[vecInd] = x_offset + (m_tileTextureSize * x) + m_tileTextureSize;
				vecInd++;
				m_vertices[vecInd] = y_offset + -m_tileTextureSize * y;
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
			}
		}
	}

	m_bounds["U"] = 0 + (m_tileTextureSize / 2);
	m_bounds["D"] = -(m_tileTextureSize * m_height) + (m_tileTextureSize / 2);
	m_bounds["L"] = 0 - (m_tileTextureSize / 2);
	m_bounds["R"] = (m_tileTextureSize * m_width) - (m_tileTextureSize / 2);
}

void TileMap::setupColliders() {
	m_parent->addComponent<Rigidbody2D>()->setRBType(STATIC);

	/*
	// Island Tracing, but I gave up
	std::vector<glm::vec3> allSolid;
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			if (m_levelDataRaw[y * m_width + x] != 0) {
				allSolid.push_back({ x, -y, getPosition().z });
			}
		}
	}

	std::vector<std::vector<glm::vec3>> islands;
	while (allSolid.size() > 0) {
		std::vector<glm::vec3> island({ allSolid[0] });
		allSolid.erase(allSolid.begin + 0);

	}
	*/

	// Lazy Layer Algorithm
	bool solidFlag = false;
	glm::vec3 startPos({ 0.0f });
	glm::vec3 finalPos({ 0.0f });
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			finalPos = { x, -y, getPosition().z };
			if ((m_levelDataRaw[y * m_width + x] != 0) && (!solidFlag)) {
				startPos = { x - 1.0f, -y, 1.0f };
				solidFlag = true;
			}
			else if ((solidFlag) && (m_levelDataRaw[y * m_width + x] == 0)) {
				m_parent->addComponent<PolygonCollider2D>(
					new PolygonCollider2D(m_parent,
						{   
							{ startPos.x + 0.5f,  startPos.y + 0.5f, startPos.z },
							{ finalPos.x - 0.5f,  startPos.y + 0.5f, startPos.z },
							{ finalPos.x - 0.5f,  startPos.y - 0.5f, startPos.z },
							{ startPos.x + 0.5f,  startPos.y - 0.5f, startPos.z },
						}));
				solidFlag = false;
			}
		}

		if (solidFlag) {
			m_parent->addComponent<PolygonCollider2D>(
				new PolygonCollider2D(m_parent,
					{   { startPos.x + 0.5f,  startPos.y + 0.5f, startPos.z },
						{ finalPos.x - 0.5f,  startPos.y + 0.5f, startPos.z },
						{ finalPos.x - 0.5f,  finalPos.y - 0.5f, startPos.z },
						{ startPos.x + 0.5f,  finalPos.y - 0.5f, startPos.z },
					}));
			solidFlag = false;
		}
	}
}

void TileMap::render(ShaderProgram* program)
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

bool TileMap::isSolid(glm::vec3 position)
{
	// If we are out of bounds, it is solid
	if (position.y > m_bounds["U"] || position.y < m_bounds["D"]) return true;
	if (position.x < m_bounds["L"] || position.x > m_bounds["R"])  return true;

	//int tileX = floor((position.x + (m_tileTextureSize / 2)) / m_tileTextureSize);
	int tileX = floor((round(position.x) + (m_tileTextureSize / 2)) / m_tileTextureSize);
	int tileY = -(ceil(position.y - (m_tileTextureSize / 2))) / m_tileTextureSize; // Our array counts up as Y goes down.

	// If the tile index is negative or greater than the dimensions, it is not solid
	if (tileX < 0 || tileX >= m_width)  return true;
	if (tileY < 0 || tileY >= m_height) return true;

	// If the tile index is not 0, it is solid
	if (m_levelDataRaw[tileY * m_width + tileX] != 0) return true;

	return false;
}

void TileMap::getTile(glm::vec3 position, glm::vec3* tilePosition) {
	if (position.y > m_bounds["U"] || position.y < m_bounds["D"]) return;
	if (position.x < m_bounds["L"] || position.x > m_bounds["R"])  return;
	int tileX = floor((round(position.x) + (m_tileTextureSize / 2)) / m_tileTextureSize);
	int tileY = -(ceil(position.y - (m_tileTextureSize / 2))) / m_tileTextureSize; // Our array counts up as Y goes down.
	if (tileX < 0 || tileX >= m_width)  return;
	if (tileY < 0 || tileY >= m_height) return;

	tilePosition->x = tileX;
	tilePosition->y = tileY;
}

void TileMap::update(float deltaTime) {
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

			build();
		}
	}
}