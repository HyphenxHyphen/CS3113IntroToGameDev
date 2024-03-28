#pragma once
#define GL_SILENCE_DEPRECATION
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include <map>  
#include <math.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Renderable.h"

class GameMap : public Renderable
{
private:
	bool m_collidable;
	bool m_pit;
	int m_width;
	int m_height;

	static const int FRAMES_PER_SECOND = 12;
	std::map<unsigned int, std::vector<int>> m_anims; // List of Animations
	unsigned int m_animIndex = 0; // Current Anim Index
	unsigned int m_animActive;
	float m_animTime = 0.0f;
	unsigned int m_animLCM;

	std::vector<unsigned int> m_levelDataRaw;
	std::vector<unsigned int> m_levelDataProcessed;
	std::vector<unsigned int> m_levelDataAnimated;

	GLuint m_tileTextureID;
	float m_tileTextureSize;
	int m_tileTextureCols;
	int m_tileTextureRows;

	std::map<std::string, int> m_tileTextureRules =
	{   {"TL", 0 }, // Top Left
		{"TC", 0 }, // Top Center
		{"TR", 0 }, // Top Right
		{"ML", 0 }, // Middle Left
		{"MC", 0 }, // Middle Center
		{"MR", 0 }, // Middle Right
		{"BL", 0 }, // Bottom Left
		{"BC", 0 }, // Bottom Center
		{"BR", 0 }, // Bottom Right
		{"VT", 0 }, // Vertical Top
		{"VM", 0 }, // Vertical Middle
		{"VB", 0 }, // Vertical Bottom
		{"HL", 0 }, // Horizontal Left
		{"HC", 0 }, // Horizontal Center
		{"HR", 0 }, // Horizontal Right
		{"[]", 0 }, // Isolated
	}; // For copy-paste

	// Just like with rendering text, we're rendering several sprites at once
	// So we need vectors to store their respective vertices and texture coordinates
	std::vector<float> m_vertices;
	std::vector<float> m_textureCoordinates;

	// The boundaries of the map
	std::map<std::string, float> m_bounds = {
		{"U", 0.0f },
		{"D", 0.0f },
		{"L", 0.0f },
		{"R", 0.0f },
	};

public:
	//float m_elevation;

	// Constructor
	GameMap();
	GameMap(bool collidable, bool pit, float elevation, int width, int height, unsigned int animIndex, std::map<unsigned int, std::vector<int>> anims, unsigned int startingAnim,
		std::vector<unsigned int> levelData, GLuint tileTextureID, float tileTextureSize, int tileTextureCols, int tileTextureRows, std::map<std::string, int> tileTextureRules);

	// Methods
	std::vector<unsigned int> GameMap::processLevelData(std::vector<unsigned int> levelDataRaw);
	void build();
	void render(ShaderProgram* program);
	bool isSolid(glm::vec3 position, float* penX, float* penY);
	bool isPit(glm::vec3 position);
	void getTile(glm::vec3 position, glm::vec3* tilePosition);
	void update(float deltaTime);

	// Getters
	bool getCollidable() { return m_collidable; }
	bool isPit() { return m_pit; }

	int getWidth() { return m_width; }
	int getHeight() { return m_height; }

	std::vector<unsigned int> getLevelDataRaw() { return m_levelDataRaw; }
	std::vector<unsigned int> getLevelDataProcessed() { return m_levelDataProcessed; }
	GLuint getTileTextureID() { return m_tileTextureID; }

	float getTileSize() { return m_tileTextureSize; }
	int getTileCols() { return m_tileTextureCols; }
	int getTileRows() { return m_tileTextureRows; }

	std::vector<float> getVertices() { return m_vertices; }
	std::vector<float> getTextureCoordinates() { return m_textureCoordinates; }

	float getBound(const std::string& bound) { return m_bounds[bound]; }
};

