/**
* Author: John Li
* Assignment: Rise of the AI
* Date due: 2024-03-30, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#define FIXED_TIMESTEP 0.0166666f
#define GL_SILENCE_DEPRECATION
//#define GL_GLEXT_PROTOTYPES 1

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <vector>
#include <ctime>
#include "Entity.h"
#include "Effect.h"
#include "GameMap.h"
#include "Util.h"
#include "Level.h"

// ————— STRUCTS AND ENUMS —————//
enum GameMode { MAIN_MENU, GAME_LEVEL, GAME_OVER };

struct GameState {
	Entity font;
	GameMode gameMode = MAIN_MENU;
	bool victory = false;
	std::map<std::string, Entity> allEntities = {};
	unsigned int currentLevelIndex = 0;
	std::vector<Level> allLevel = {};
	std::vector<std::vector<GameMap>> allLevelData = {};
	std::map<std::string, Entity*> allEnemies = {};
	std::map<std::string, Entity*> allRed = {};
	std::map<std::string, Entity*> allPlayers = {};
	std::vector<std::string> entitiesGarbageCollection = {};
	std::vector<Effect> allEffects = {};
	float gameEndTimer = 0.0f;
	glm::vec3 cameraPos = {0.0f, 0.0f, 0.0f};
};

// Window
const int WINDOW_WIDTH = 640 * 2.0f;
const int WINDOW_HEIGHT = 480 * 2.0f;

// BG
float g_bgR =  71.0f / 255.0f;
float g_bgG = 171.0f / 255.0f;
float g_bgB = 171.0f / 255.0f;
float g_bgA = 1.0f;

// Camera
const int VIEWPORT_X = 0;
const int VIEWPORT_Y = 0;
const int VIEWPORT_WIDTH = WINDOW_WIDTH;
const int VIEWPORT_HEIGHT = WINDOW_HEIGHT;
float cameraScale = 1.5f;

// Shader filepaths
const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl";
const char F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

// Display Globals
SDL_Window* g_displayWindow;

// Game Globals
float ACC_OF_GRAVITY = -0.0f;
GameState g_gameState;

bool g_gameIsRunning = true;

float g_seconds;
float g_previousSeconds;
float g_deltaTime;
float g_previousTicks;
float g_timeAccumulator;

// Shader Globals
ShaderProgram g_program;
glm::mat4 g_viewMatrix;        // Defines camera location and orientation
glm::mat4 g_projectionMatrix;  // Defines camera clip panes, field of view, projection method, etc.

void initialize();
void resetGame();
void processInput();
void processInputMainMenu();
void processInputGameLevel();
void processInputGameOver();
void update();
void updateMainMenu();
void updateGameLevel();
void updateGameOver();
void updateDeltaTime();
void render();
void shutdown();

int main(int argc, char* argv[]) {
	initialize();
	while (g_gameIsRunning) {
		processInput();
		update();
		render();
	}
	shutdown();
	return 0;
}

void initialize()
{
	SDL_Init(SDL_INIT_VIDEO);
	g_displayWindow = SDL_CreateWindow("Hello, Goblin!",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL);

	SDL_GLContext context = SDL_GL_CreateContext(g_displayWindow);
	SDL_GL_MakeCurrent(g_displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

	g_program.load(V_SHADER_PATH, F_SHADER_PATH);

	// Camera properties
	g_viewMatrix = glm::mat4(1.0f);  // Defines the position (location and orientation) of the camera
	g_projectionMatrix = glm::ortho(-((WINDOW_WIDTH / 320.0f) * 1.25f * cameraScale), ((WINDOW_WIDTH / 320.0f) * 1.25f * cameraScale), 
									-((WINDOW_HEIGHT / 320.0f) * 1.25f * cameraScale), ((WINDOW_HEIGHT / 320.0f) * 1.25f * cameraScale), 
		-1.0f, 1.0f);  // Defines the characteristics of your camera, such as clip planes, field of view, projection method etc.

	g_program.set_projection_matrix(g_projectionMatrix);
	g_program.set_view_matrix(g_viewMatrix);

	glUseProgram(g_program.get_program_id());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(g_bgR, g_bgG, g_bgB, g_bgA);

	// Initialize Game State

	// Read Image?

	//std::cout << "-----" << std::endl;


	// Font Texture
	g_gameState.font = Entity("Font", Util::loadTexture("Assets/FontAtlas2.png"), 5, 9, 0,
		{},
		0,
		0.0f, 0.0f, 0.0f, 0.0f,
		glm::vec3(1.0f, 1.0f, 1.0f)
	);

	// Map Grid Setup
	//GameMap(int width, int height, std::vector<unsigned int> levelData, std::vector<GLuint> tileTextureIDs, float tileSize, int tileCols, int tileRows);
	Level::initialize(g_gameState.allLevelData);
	g_gameState.currentLevelIndex = 0;

	// Reset Game
	resetGame();
}

void resetGame() {
	g_gameState.gameEndTimer = 0.0f;
	g_gameState.victory = false;
	g_gameState.allEntities.clear();
	g_gameState.allEffects.clear();
	g_gameState.allPlayers.clear();
	g_gameState.allEnemies.clear();
	g_gameState.allRed.clear();
	
	srand(time(NULL)); // Reset Random

	// Initialize Objects (models and assets)

	if (g_gameState.currentLevelIndex == 0) {
		// Player
		g_gameState.allEntities["Player"] = Entity("Player", Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Warrior/Blue/Warrior_Blue.png"), 6, 8, 0,
			{ { 0,  1,  2,  3,  4,  5},
			  { 6,  7,  8,  9, 10, 11},
			  {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
			  {24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
			  {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
			},
			0, 5, 2, 2.5f, 1.0f,
			glm::vec3(0.5f, 0.5f, 1.0f)
		);
		g_gameState.allEntities["Player"].setPosition(glm::vec3(3.5f, -5.5f, 0.0f));
		g_gameState.allEntities["Player"].update(g_gameState.allLevelData[g_gameState.currentLevelIndex], g_gameState.allRed, g_gameState.allEnemies, g_gameState.allEnemies, &g_gameState.allEffects, FIXED_TIMESTEP);
		g_gameState.allPlayers["Player"] = &g_gameState.allEntities["Player"];

		// Move Camera to Player
		g_gameState.cameraPos = glm::vec3(g_gameState.allEntities["Player"].getPosition().x, g_gameState.allEntities["Player"].getPosition().y, 0.0f);
		g_viewMatrix = glm::mat4(1.0f);
		g_viewMatrix = glm::translate(g_viewMatrix, glm::vec3(-g_gameState.cameraPos.x, -g_gameState.cameraPos.y, g_gameState.cameraPos.z));
		g_program.set_view_matrix(g_viewMatrix);


		// Enemy
		std::string enemyName = "EnemyGuard";
		for (int enemy = 1; enemy < 4; enemy++) {
			std::string tempEnemyName = enemyName + std::to_string(enemy);

			g_gameState.allEntities[tempEnemyName] = Entity(tempEnemyName, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Warrior/Red/Warrior_Red.png"), 6, 8, 0,
				{ { 0,  1,  2,  3,  4,  5},
				  { 6,  7,  8,  9, 10, 11},
				  {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
				  {24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
				  {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
				},
				0, 5, 1, 2.0f, 1.0f,
				glm::vec3(0.5f, 0.5f, 1.0f)
			);
			g_gameState.allEntities[tempEnemyName].setPosition(glm::vec3(10.0f, -4.5f - (enemy * 0.5f), 0.0f));
			g_gameState.allEntities[tempEnemyName].setRotation({ g_gameState.allEntities[tempEnemyName].getRotation().x, 180.0f, g_gameState.allEntities[tempEnemyName].getRotation().z });
			g_gameState.allEntities[tempEnemyName].update(g_gameState.allLevelData[g_gameState.currentLevelIndex], g_gameState.allRed, g_gameState.allPlayers, g_gameState.allPlayers, &g_gameState.allEffects, FIXED_TIMESTEP);
			g_gameState.allRed[tempEnemyName] = &g_gameState.allEntities[tempEnemyName];
			g_gameState.allEntities[tempEnemyName].m_aiType = GUARD;
			g_gameState.allEntities[tempEnemyName].m_target = &g_gameState.allEntities["Player"];
		}

		enemyName = "EnemyGoblin";
		std::vector<glm::vec3> enemySpawnPositions = {
			{14.5f, -5.5f, 0.0f},
		};
		for (int enemy = 0; enemy < enemySpawnPositions.size(); enemy++) {
			std::string tempEnemyName = enemyName + std::to_string(enemy + 5);
			g_gameState.allEntities[tempEnemyName] = Entity(tempEnemyName, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Goblins/Troops/Torch/Red/Torch_Red.png"), 7, 5, 0,
				{ { 0,  1,  2,  3,  4,  5, 6},
				  { 7,  8,  9, 10, 11, 12},
				  {14, 15, 16, 17, 18},
				  {21, 22, 23, 24, 25},
				  {28, 29, 30, 31, 32},
				},
				0, 5, 3, 3.0f, 1.0f,
				glm::vec3(0.5f, 0.5f, 1.0f)
			);
			g_gameState.allEntities[tempEnemyName].setPosition(enemySpawnPositions[enemy]);
			g_gameState.allEntities[tempEnemyName].setRotation({ g_gameState.allEntities[tempEnemyName].getRotation().x, (enemySpawnPositions[enemy].x == 16) ? 0.0f : 180.0f, g_gameState.allEntities[tempEnemyName].getRotation().z });
			g_gameState.allEntities[tempEnemyName].update(g_gameState.allLevelData[g_gameState.currentLevelIndex], g_gameState.allRed, g_gameState.allPlayers, g_gameState.allPlayers, &g_gameState.allEffects, FIXED_TIMESTEP);
			g_gameState.allEnemies[tempEnemyName] = &g_gameState.allEntities[tempEnemyName];
			g_gameState.allEntities[tempEnemyName].m_aiType = GOBLIN;
			g_gameState.allEntities[tempEnemyName].m_target = &g_gameState.allEntities["Player"];
			g_gameState.allEntities[tempEnemyName].m_evil = true;
		}
	}
	else if (g_gameState.currentLevelIndex == 1) {
		// Player
		g_gameState.allEntities["Player"] = Entity("Player", Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Warrior/Blue/Warrior_Blue.png"), 6, 8, 0,
			{ { 0,  1,  2,  3,  4,  5},
			  { 6,  7,  8,  9, 10, 11},
			  {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
			  {24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
			  {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
			},
			0, 5, 2, 2.5f, 1.0f,
			glm::vec3(0.5f, 0.5f, 1.0f)
		);
		g_gameState.allEntities["Player"].setPosition(glm::vec3(5.0f, -6.0f, 0.0f));
		g_gameState.allEntities["Player"].update(g_gameState.allLevelData[g_gameState.currentLevelIndex], g_gameState.allRed, g_gameState.allEnemies, g_gameState.allEnemies, &g_gameState.allEffects, FIXED_TIMESTEP);
		g_gameState.allPlayers["Player"] = &g_gameState.allEntities["Player"];

		// Move Camera to Player
		g_gameState.cameraPos = glm::vec3(g_gameState.allEntities["Player"].getPosition().x, g_gameState.allEntities["Player"].getPosition().y, 0.0f);
		g_viewMatrix = glm::mat4(1.0f);
		g_viewMatrix = glm::translate(g_viewMatrix, glm::vec3(-g_gameState.cameraPos.x, -g_gameState.cameraPos.y, g_gameState.cameraPos.z));
		g_program.set_view_matrix(g_viewMatrix);

		// Enemy
		std::string enemyName = "EnemyVillager";
		std::vector<glm::vec3> enemySpawnPositions = {
			{15.0f, -3.0f, 0.0f},
			{13.0f, -4.0f, 0.0f},
			{17.0f, -4.0f, 0.0f},
			{12.0f, -6.0f, 0.0f},
			{15.0f, -6.0f, 0.0f},
			{18.0f, -6.0f, 0.0f},
			{13.0f, -8.0f, 0.0f},
			{17.0f, -8.0f, 0.0f},
			{15.0f, -9.0f, 0.0f},
		};
		for (int enemy = 0; enemy < enemySpawnPositions.size(); enemy++) {
			std::string tempEnemyName = enemyName + std::to_string(enemy + 1);
			g_gameState.allEntities[tempEnemyName] = Entity(tempEnemyName, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Pawn/Red/Pawn_Red.png"), 6, 6, 0,
				{ { 0,  1,  2,  3,  4,  5},
				  { 6,  7,  8,  9, 10, 11},
				  {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
				  {24, 25, 26, 27, 28, 29},
				  {30, 31, 32, 33, 34, 35},
				},
				0, 3, 1, 1.5f, 1.0f,
				glm::vec3(0.5f, 0.5f, 1.0f)
			);
			g_gameState.allEntities[tempEnemyName].setPosition(enemySpawnPositions[enemy]);
			g_gameState.allEntities[tempEnemyName].setRotation({ g_gameState.allEntities[tempEnemyName].getRotation().x, 180.0f, g_gameState.allEntities[tempEnemyName].getRotation().z });
			g_gameState.allEntities[tempEnemyName].update(g_gameState.allLevelData[g_gameState.currentLevelIndex], g_gameState.allRed, g_gameState.allPlayers, g_gameState.allPlayers, &g_gameState.allEffects, FIXED_TIMESTEP);
			g_gameState.allRed[tempEnemyName] = &g_gameState.allEntities[tempEnemyName];
			g_gameState.allEntities[tempEnemyName].m_aiType = VILLAGER;
			g_gameState.allEntities[tempEnemyName].m_target = &g_gameState.allEntities["Player"];
		}

		enemyName = "EnemyGuard";
		enemySpawnPositions = {
			{4.0f, -5.0f, 0.0f},
			{6.0f, -5.0f, 0.0f},
			{4.0f, -7.0f, 0.0f},
			{6.0f, -7.0f, 0.0f},
			{14.0f, -5.0f, 0.0f},
			{16.0f, -5.0f, 0.0f},
			{14.0f, -7.0f, 0.0f},
			{16.0f, -7.0f, 0.0f},
		};
		for (int enemy = 0; enemy < enemySpawnPositions.size(); enemy++) {
			std::string tempEnemyName = enemyName + std::to_string(enemy);
			g_gameState.allEntities[tempEnemyName] = Entity(tempEnemyName, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Warrior/Red/Warrior_Red.png"), 6, 8, 0,
				{ { 0,  1,  2,  3,  4,  5},
				  { 6,  7,  8,  9, 10, 11},
				  {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
				  {24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
				  {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
				},
				0, 5, 1, 2.0f, 1.0f,
				glm::vec3(0.5f, 0.5f, 1.0f)
			);
			g_gameState.allEntities[tempEnemyName].setPosition(enemySpawnPositions[enemy]);
			g_gameState.allEntities[tempEnemyName].setRotation({ g_gameState.allEntities[tempEnemyName].getRotation().x, ((enemySpawnPositions[enemy].x == 4) || (enemySpawnPositions[enemy].x == 16)) ? 0.0f : 180.0f, g_gameState.allEntities[tempEnemyName].getRotation().z });
			g_gameState.allEntities[tempEnemyName].update(g_gameState.allLevelData[g_gameState.currentLevelIndex], g_gameState.allRed, g_gameState.allPlayers, g_gameState.allPlayers, &g_gameState.allEffects, FIXED_TIMESTEP);
			g_gameState.allRed[tempEnemyName] = &g_gameState.allEntities[tempEnemyName];
			g_gameState.allEntities[tempEnemyName].m_aiType = GUARD;
			g_gameState.allEntities[tempEnemyName].m_target = &g_gameState.allEntities["Player"];
		}
		
		enemyName = "EnemyGoblin";
		enemySpawnPositions = {
			{24.0f, -6.0f, 0.0f},
		};
		for (int enemy = 0; enemy < enemySpawnPositions.size(); enemy++) {
			std::string tempEnemyName = enemyName + std::to_string(enemy + 5);
			g_gameState.allEntities[tempEnemyName] = Entity(tempEnemyName, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Goblins/Troops/Torch/Red/Torch_Red.png"), 7, 5, 0,
				{ { 0,  1,  2,  3,  4,  5, 6},
				  { 7,  8,  9, 10, 11, 12},
				  {14, 15, 16, 17, 18},
				  {21, 22, 23, 24, 25},
				  {28, 29, 30, 31, 32},
				},
				0, 5, 3, 3.0f, 1.0f,
				glm::vec3(0.5f, 0.5f, 1.0f)
			);
			g_gameState.allEntities[tempEnemyName].setPosition(enemySpawnPositions[enemy]);
			g_gameState.allEntities[tempEnemyName].setRotation({ g_gameState.allEntities[tempEnemyName].getRotation().x, 180.0f, g_gameState.allEntities[tempEnemyName].getRotation().z });
			g_gameState.allEntities[tempEnemyName].update(g_gameState.allLevelData[g_gameState.currentLevelIndex], g_gameState.allRed, g_gameState.allPlayers, g_gameState.allPlayers, &g_gameState.allEffects, FIXED_TIMESTEP);
			g_gameState.allEnemies[tempEnemyName] = &g_gameState.allEntities[tempEnemyName];
			g_gameState.allEntities[tempEnemyName].m_aiType = GOBLIN;
			g_gameState.allEntities[tempEnemyName].m_target = &g_gameState.allEntities["Player"];
			g_gameState.allEntities[tempEnemyName].m_evil = true;
		}
	}
	else if (g_gameState.currentLevelIndex == 2) {
		// Player
		g_gameState.allEntities["Player"] = Entity("Player", Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Warrior/Blue/Warrior_Blue.png"), 6, 8, 0,
			{ { 0,  1,  2,  3,  4,  5},
			  { 6,  7,  8,  9, 10, 11},
			  {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
			  {24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
			  {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
			},
			0, 5, 2, 2.5f, 0.0f,
			glm::vec3(0.5f, 0.5f, 1.0f)
		);
		g_gameState.allEntities["Player"].setPosition(glm::vec3(3.0f, -5.0f, 0.0f));
		g_gameState.allEntities["Player"].update(g_gameState.allLevelData[g_gameState.currentLevelIndex], g_gameState.allRed, g_gameState.allEnemies, g_gameState.allEnemies, &g_gameState.allEffects, FIXED_TIMESTEP);
		g_gameState.allPlayers["Player"] = &g_gameState.allEntities["Player"];

		// Move Camera to Player
		g_gameState.cameraPos = glm::vec3(g_gameState.allEntities["Player"].getPosition().x, g_gameState.allEntities["Player"].getPosition().y, 0.0f);
		g_viewMatrix = glm::mat4(1.0f);
		g_viewMatrix = glm::translate(g_viewMatrix, glm::vec3(-g_gameState.cameraPos.x, -g_gameState.cameraPos.y, g_gameState.cameraPos.z));
		g_program.set_view_matrix(g_viewMatrix);

		// Enemy
		std::string enemyName = "EnemyVillager";
		std::vector<glm::vec3> enemySpawnPositions = {
			{21.0f, -3.0f, 0.0f},
			{21.0f, -5.0f, 0.0f},
			{21.0f, -7.0f, 0.0f},
		};
		for (int enemy = 0; enemy < enemySpawnPositions.size(); enemy++) {
			std::string tempEnemyName = enemyName + std::to_string(enemy + 1);
			g_gameState.allEntities[tempEnemyName] = Entity(tempEnemyName, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Pawn/Red/Pawn_Red.png"), 6, 6, 0,
				{ { 0,  1,  2,  3,  4,  5},
				  { 6,  7,  8,  9, 10, 11},
				  {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
				  {24, 25, 26, 27, 28, 29},
				  {30, 31, 32, 33, 34, 35},
				},
				0, 3, 1, 1.5f, 0.0f,
				glm::vec3(0.5f, 0.5f, 1.0f)
			);
			g_gameState.allEntities[tempEnemyName].setPosition(enemySpawnPositions[enemy]);
			g_gameState.allEntities[tempEnemyName].setRotation({ g_gameState.allEntities[tempEnemyName].getRotation().x, 180.0f, g_gameState.allEntities[tempEnemyName].getRotation().z });
			g_gameState.allEntities[tempEnemyName].update(g_gameState.allLevelData[g_gameState.currentLevelIndex], g_gameState.allRed, g_gameState.allPlayers, g_gameState.allPlayers, &g_gameState.allEffects, FIXED_TIMESTEP);
			g_gameState.allRed[tempEnemyName] = &g_gameState.allEntities[tempEnemyName];
			g_gameState.allEntities[tempEnemyName].m_aiType = VILLAGER;
			g_gameState.allEntities[tempEnemyName].m_target = &g_gameState.allEntities["Player"];
		}

		enemyName = "EnemyGuard";
		enemySpawnPositions = {
			{13.0f, -3.0f, 0.0f},
			{13.0f, -5.0f, 0.0f},
			{13.0f, -7.0f, 0.0f},
		};
		for (int enemy = 0; enemy < enemySpawnPositions.size(); enemy++) {
			std::string tempEnemyName = enemyName + std::to_string(enemy);
			g_gameState.allEntities[tempEnemyName] = Entity(tempEnemyName, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Warrior/Red/Warrior_Red.png"), 6, 8, 0,
				{ { 0,  1,  2,  3,  4,  5},
				  { 6,  7,  8,  9, 10, 11},
				  {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
				  {24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
				  {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
				},
				0, 5, 1, 2.0f, 0.0f,
				glm::vec3(0.5f, 0.5f, 1.0f)
			);
			g_gameState.allEntities[tempEnemyName].setPosition(enemySpawnPositions[enemy]);
			g_gameState.allEntities[tempEnemyName].setRotation({ g_gameState.allEntities[tempEnemyName].getRotation().x, 180.0f, g_gameState.allEntities[tempEnemyName].getRotation().z });
			g_gameState.allEntities[tempEnemyName].update(g_gameState.allLevelData[g_gameState.currentLevelIndex], g_gameState.allRed, g_gameState.allPlayers, g_gameState.allPlayers, &g_gameState.allEffects, FIXED_TIMESTEP);
			g_gameState.allRed[tempEnemyName] = &g_gameState.allEntities[tempEnemyName];
			g_gameState.allEntities[tempEnemyName].m_aiType = GUARD;
			g_gameState.allEntities[tempEnemyName].m_target = &g_gameState.allEntities["Player"];
		}

		enemyName = "EnemyGoblin";
		enemySpawnPositions = {
			{29.0f, -5.0f, 0.0f},
			{37.0f, -4.0f, 0.0f},
			{37.0f, -6.0f, 0.0f},
			{45.0f, -3.0f, 0.0f},
			{45.0f, -5.0f, 0.0f},
			{45.0f, -7.0f, 0.0f},
		};
		for (int enemy = 0; enemy < enemySpawnPositions.size(); enemy++) {
			std::string tempEnemyName = enemyName + std::to_string(enemy + 5);
			g_gameState.allEntities[tempEnemyName] = Entity(tempEnemyName, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Goblins/Troops/Torch/Red/Torch_Red.png"), 7, 5, 0,
				{ { 0,  1,  2,  3,  4,  5, 6},
				  { 7,  8,  9, 10, 11, 12},
				  {14, 15, 16, 17, 18},
				  {21, 22, 23, 24, 25},
				  {28, 29, 30, 31, 32},
				},
				0, 5, 3, 3.0f, 0.0f,
				glm::vec3(0.5f, 0.5f, 1.0f)
			);
			g_gameState.allEntities[tempEnemyName].setPosition(enemySpawnPositions[enemy]);
			g_gameState.allEntities[tempEnemyName].setRotation({ g_gameState.allEntities[tempEnemyName].getRotation().x, 180.0f, g_gameState.allEntities[tempEnemyName].getRotation().z });
			g_gameState.allEntities[tempEnemyName].update(g_gameState.allLevelData[g_gameState.currentLevelIndex], g_gameState.allRed, g_gameState.allPlayers, g_gameState.allPlayers, &g_gameState.allEffects, FIXED_TIMESTEP);
			g_gameState.allEnemies[tempEnemyName] = &g_gameState.allEntities[tempEnemyName];
			g_gameState.allEntities[tempEnemyName].m_aiType = GOBLIN;
			g_gameState.allEntities[tempEnemyName].m_target = &g_gameState.allEntities["Player"];
			g_gameState.allEntities[tempEnemyName].m_evil = true;
		}
	}
	g_gameState.gameMode = MAIN_MENU;
}

void processInput()
{
	switch (g_gameState.gameMode)
	{
	case MAIN_MENU:
		processInputMainMenu();
		break;

	case GAME_LEVEL:
		processInputGameLevel();
		break;

	case GAME_OVER:
		processInputGameOver();
		break;
	}
}

void processInputMainMenu() {
	// Single Press
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			g_gameIsRunning = false;
			break;
		case SDL_KEYDOWN:
			g_gameState.gameMode = GAME_LEVEL;
		}
	}

	// Hold
	const Uint8* key_state = SDL_GetKeyboardState(NULL);

}

void processInputGameLevel() {
	// Single Press
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			g_gameIsRunning = false;
			break;
		case SDL_MOUSEWHEEL:
			if (event.wheel.y > 0) // scroll up
			{
				cameraScale -= 0.1;
			}
			else if (event.wheel.y < 0) // scroll down
			{
				cameraScale += 0.1;
			}
			break;
		}
	}

	// Hold
	const Uint8* key_state = SDL_GetKeyboardState(NULL);

	// Moving
	if (g_gameState.allEntities.count("Player")) {
		if ((g_gameState.allEntities["Player"].getActionState() == STUNNED) && (g_gameState.allEntities["Player"].m_carrier != nullptr)) {
			if ((key_state[SDL_SCANCODE_A]) && (key_state[SDL_SCANCODE_D]))
			{
				// Nothing
			}
			else if (key_state[SDL_SCANCODE_A] && (g_gameState.allEntities["Player"].getRotation().y == 0.0f))
			{
				g_gameState.allEntities["Player"].setRotation({ g_gameState.allEntities["Player"].getRotation().x, 180.0f, g_gameState.allEntities["Player"].getRotation().z });
				if ((rand() % 100) < 20) {
					g_gameState.allEntities["Player"].m_carrier = nullptr;
					g_gameState.allEntities["Player"].jump();
				}
			}
			else if (key_state[SDL_SCANCODE_D] && (g_gameState.allEntities["Player"].getRotation().y == 180.0f))
			{
				g_gameState.allEntities["Player"].setRotation({ g_gameState.allEntities["Player"].getRotation().x, 0.0f, g_gameState.allEntities["Player"].getRotation().z });
				if ((rand() % 100) < 20){
					g_gameState.allEntities["Player"].m_carrier->setActionState(MOVING);
					g_gameState.allEntities["Player"].m_carrier = nullptr;
					g_gameState.allEntities["Player"].jump();
				}
			}
			else
			{
				// Nothing
			}
		}
		else if ((g_gameState.allEntities["Player"].getActionState() == MOVING) || (g_gameState.allEntities["Player"].getActionState() == IDLE)) {
			if ((key_state[SDL_SCANCODE_W]) && (key_state[SDL_SCANCODE_S]))
			{
				g_gameState.allEntities["Player"].setVelocity(
					glm::vec3(
						g_gameState.allEntities["Player"].getVelocity().x,
						0.0f,
						g_gameState.allEntities["Player"].getVelocity().z
					)
				);
			}
			else if (key_state[SDL_SCANCODE_W])
			{
				g_gameState.allEntities["Player"].setVelocity(
					glm::vec3(
						g_gameState.allEntities["Player"].getVelocity().x,
						1.0f,
						g_gameState.allEntities["Player"].getVelocity().z
					)
				);
			}
			else if (key_state[SDL_SCANCODE_S])
			{
				g_gameState.allEntities["Player"].setVelocity(
					glm::vec3(
						g_gameState.allEntities["Player"].getVelocity().x,
						-1.0f,
						g_gameState.allEntities["Player"].getVelocity().z
					)
				);
			}
			else
			{
				g_gameState.allEntities["Player"].setVelocity(
					glm::vec3(
						g_gameState.allEntities["Player"].getVelocity().x,
						0.0f,
						g_gameState.allEntities["Player"].getVelocity().z
					)
				);
			}

			if ((key_state[SDL_SCANCODE_A]) && (key_state[SDL_SCANCODE_D]))
			{
				g_gameState.allEntities["Player"].setVelocity(
					glm::vec3(
						0.0f,
						g_gameState.allEntities["Player"].getVelocity().y,
						g_gameState.allEntities["Player"].getVelocity().z
					)
				);
			}
			else if (key_state[SDL_SCANCODE_A])
			{
				g_gameState.allEntities["Player"].setVelocity(
					glm::vec3(
						-1.0f,
						g_gameState.allEntities["Player"].getVelocity().y,
						g_gameState.allEntities["Player"].getVelocity().z
					)
				);
				g_gameState.allEntities["Player"].setRotation({ g_gameState.allEntities["Player"].getRotation().x, 180.0f, g_gameState.allEntities["Player"].getRotation().z });
			}
			else if (key_state[SDL_SCANCODE_D])
			{
				g_gameState.allEntities["Player"].setVelocity(
					glm::vec3(
						1.0f,
						g_gameState.allEntities["Player"].getVelocity().y,
						g_gameState.allEntities["Player"].getVelocity().z
					)
				);
				g_gameState.allEntities["Player"].setRotation({ g_gameState.allEntities["Player"].getRotation().x, 0.0f, g_gameState.allEntities["Player"].getRotation().z });
			}
			else
			{
				g_gameState.allEntities["Player"].setVelocity(
					glm::vec3(
						0.0f,
						g_gameState.allEntities["Player"].getVelocity().y,
						g_gameState.allEntities["Player"].getVelocity().z
					)
				);
			}

			// Normalization

			if (glm::length(g_gameState.allEntities["Player"].getVelocity()) > 1.0f)
			{
				g_gameState.allEntities["Player"].setVelocity(glm::normalize(g_gameState.allEntities["Player"].getVelocity()));
			}

			if ((g_gameState.allEntities["Player"].getVelocity().x != 0) || (g_gameState.allEntities["Player"].getVelocity().y != 0)) {
				g_gameState.allEntities["Player"].setActionState(MOVING);
			}
			else {
				g_gameState.allEntities["Player"].setActionState(IDLE);
			}

			if (key_state[SDL_SCANCODE_SPACE]) {
				g_gameState.allEntities["Player"].jump();
			}

			if (g_gameState.allEntities["Player"].getActionState() == MOVING) {
				if (g_gameState.allEntities["Player"].getAnimActive() != 1) {
					g_gameState.allEntities["Player"].setAnimActive(1);
				}
			}
			else if (g_gameState.allEntities["Player"].getActionState() == IDLE) {
				if (g_gameState.allEntities["Player"].getAnimActive() != 0) {
					g_gameState.allEntities["Player"].setAnimActive(0);
				}
			}
		}

		// Attack Overrides Movement
		if (g_gameState.allEntities["Player"].getActionState() == MOVING) {
			if (key_state[SDL_SCANCODE_RIGHT] || key_state[SDL_SCANCODE_LEFT] || key_state[SDL_SCANCODE_DOWN] || key_state[SDL_SCANCODE_UP]) {
				g_gameState.allEntities["Player"].setActionState(ATTACKING);
				g_gameState.allEntities["Player"].setVelocity(
					glm::vec3(
						0.0f,
						0.0f,
						g_gameState.allEntities["Player"].getVelocity().z
					)
				);
			}
		}

		// Attacking
		if ((g_gameState.allEntities["Player"].getActionState() == ATTACKING) || (g_gameState.allEntities["Player"].getActionState() == IDLE)) {
			if (key_state[SDL_SCANCODE_RIGHT]) {
				g_gameState.allEntities["Player"].setActionState(ATTACKING);
				g_gameState.allEntities["Player"].setFaceDirection(RIGHT);
				g_gameState.allEntities["Player"].setRotation({ g_gameState.allEntities["Player"].getRotation().x, 0.0f, g_gameState.allEntities["Player"].getRotation().z });
				if (g_gameState.allEntities["Player"].getAnimActive() != 2) {
					g_gameState.allEntities["Player"].setAnimActive(2);
				}
			}
			else if (key_state[SDL_SCANCODE_LEFT]) {
				g_gameState.allEntities["Player"].setActionState(ATTACKING);
				g_gameState.allEntities["Player"].setFaceDirection(LEFT);
				g_gameState.allEntities["Player"].setRotation({ g_gameState.allEntities["Player"].getRotation().x, 180.0f, g_gameState.allEntities["Player"].getRotation().z });
				if (g_gameState.allEntities["Player"].getAnimActive() != 2) {
					g_gameState.allEntities["Player"].setAnimActive(2);
				}
			}
			else if (key_state[SDL_SCANCODE_DOWN]) {
				g_gameState.allEntities["Player"].setActionState(ATTACKING);
				g_gameState.allEntities["Player"].setFaceDirection(DOWN);
				if (g_gameState.allEntities["Player"].getAnimActive() != 3) {
					g_gameState.allEntities["Player"].setAnimActive(3);
				}
			}
			else if (key_state[SDL_SCANCODE_UP]) {
				g_gameState.allEntities["Player"].setActionState(ATTACKING);
				g_gameState.allEntities["Player"].setFaceDirection(UP);
				if (g_gameState.allEntities["Player"].getAnimActive() != 4) {
					g_gameState.allEntities["Player"].setAnimActive(4);
				}
			}
			else {
				g_gameState.allEntities["Player"].setActionState(IDLE);
				if (g_gameState.allEntities["Player"].getAnimActive() != 0) {
					g_gameState.allEntities["Player"].setAnimActive(0);
				}
			}
		}
	}
}

void processInputGameOver() {
	// Single Press
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			g_gameIsRunning = false;
			break;
		case SDL_KEYDOWN:
			if (g_gameState.victory) {
				if (g_gameState.currentLevelIndex < g_gameState.allLevelData.size() - 1) {
					g_gameState.currentLevelIndex += 1;
				}
				else {
					g_gameState.currentLevelIndex = 0;
				}
			}
			resetGame();
		}
	}

	// Hold
	const Uint8* key_state = SDL_GetKeyboardState(NULL);
}

void update() {
	updateDeltaTime();
	//std::cout << (int)g_seconds << std::endl;

	g_deltaTime += g_timeAccumulator;

	// Clean up allEffects
	int iNow = 0;
	int iMax = g_gameState.allEffects.size();
	while (iNow < iMax) {
		//std::cout << "Cleaning " << iNow << " " << iMax << std::endl;
		if (!g_gameState.allEffects[iNow].m_isAlive) {
			std::swap(g_gameState.allEffects[iNow], g_gameState.allEffects[iMax - 1]);
			g_gameState.allEffects.pop_back();
			iMax--;
			//std::cout << "Cleaned" << std::endl;
		}
		else {
			iNow += 1;
			//std::cout << "Next " << iNow << std::endl;
		}
	}

	// Clean up entities
	for (auto& entityPair : g_gameState.allEntities) {
		if (!entityPair.second.m_isAlive) {
			g_gameState.entitiesGarbageCollection.push_back(entityPair.first);
			if (g_gameState.allPlayers.count(entityPair.first)) {
				g_gameState.allPlayers.erase(entityPair.first);
			}
			if (g_gameState.allEnemies.count(entityPair.first)) {
				g_gameState.allEnemies.erase(entityPair.first);
			}
			if (g_gameState.allRed.count(entityPair.first)) {
				g_gameState.allRed.erase(entityPair.first);
			}
			for (auto& entityPairOther : g_gameState.allEntities) {
				if (entityPairOther.second.m_collidingWith.count(entityPair.first)) {
					entityPairOther.second.m_collidingWith.erase(entityPair.first);
				}
				if (entityPairOther.second.m_target == &entityPair.second) {
					entityPairOther.second.m_target = nullptr;
				}
			}
		}
	}

	/*
	for (auto& entityKey : g_gameState.entitiesGarbageCollection) {
		//std::cout << "Ready to Clean " << entityKey << g_gameState.allEntities.count(entityKey) << std::endl;
		g_gameState.allEntities.erase(entityKey);
		//std::cout << "Cleaned " << entityKey << g_gameState.allEntities.count(entityKey) << std::endl;
	}
	*/

	// Fixed Update Below
	if (g_deltaTime < FIXED_TIMESTEP)
	{
		g_timeAccumulator = g_deltaTime;
		return;
	}

	//std::cout << "Lag:" << g_deltaTime << std::endl;


	while (g_deltaTime >= FIXED_TIMESTEP)
	{
		switch (g_gameState.gameMode)
		{
		case MAIN_MENU:
			updateMainMenu();
			break;

		case GAME_LEVEL:
			updateGameLevel();
			break;

		case GAME_OVER:
			updateGameOver();
			break;
		}

		g_deltaTime -= FIXED_TIMESTEP;
	}

	g_timeAccumulator = g_deltaTime;
}

void updateDeltaTime() {
	float ticks = (float)SDL_GetTicks() / 1000;  // get the current number of ticks by millisecond per second
	g_deltaTime = ticks - g_previousTicks;     // delta time = change in time since last frame
	g_previousTicks = ticks;
	g_seconds += 1.0f * g_deltaTime;    // get the current number of seconds that have passed
}

void updateMainMenu() {

}

void updateGameLevel() {
	for (auto& entityPair : g_gameState.allEntities) {
		if (entityPair.second.m_isAlive) {
			if (entityPair.first == "Player") {
				std::map<std::string, Entity*> npc = {};
				npc.insert(g_gameState.allRed.begin(), g_gameState.allRed.end());
				npc.insert(g_gameState.allEnemies.begin(), g_gameState.allEnemies.end());
				entityPair.second.update(g_gameState.allLevelData[g_gameState.currentLevelIndex], g_gameState.allRed, g_gameState.allEnemies, npc, &g_gameState.allEffects, FIXED_TIMESTEP);
				//std::cout << "Player state " << g_gameState.allEntities["Player"].m_actionState << std::endl;
			}
			else {
				entityPair.second.update(g_gameState.allLevelData[g_gameState.currentLevelIndex], g_gameState.allRed, g_gameState.allPlayers, g_gameState.allPlayers, &g_gameState.allEffects, FIXED_TIMESTEP);
			}
		}
	}

	for (auto& effect : g_gameState.allEffects) {
		effect.update(FIXED_TIMESTEP);
	}

	for (auto& map : g_gameState.allLevelData[g_gameState.currentLevelIndex]) {
		map.update(FIXED_TIMESTEP);
	}

	// Move Camera to Player
	if (g_gameState.allEntities.count("Player")) {
		//std::cout << "Player alive? " << g_gameState.allEntities["Player"].getPosition().x << ", " << g_gameState.allEntities["Player"].getPosition().y << std::endl;
		g_gameState.cameraPos = glm::vec3(g_gameState.allEntities["Player"].getPosition().x, g_gameState.allEntities["Player"].getPosition().y, 0.0f);
		g_viewMatrix = glm::mat4(1.0f);
		g_viewMatrix = glm::translate(g_viewMatrix, glm::vec3(-g_gameState.cameraPos.x, -g_gameState.cameraPos.y, g_gameState.cameraPos.z));
		g_program.set_view_matrix(g_viewMatrix);
		g_projectionMatrix = glm::ortho(-((WINDOW_WIDTH / 320.0f) * 1.25f * cameraScale), ((WINDOW_WIDTH / 320.0f) * 1.25f * cameraScale),
			-((WINDOW_HEIGHT / 320.0f) * 1.25f * cameraScale), ((WINDOW_HEIGHT / 320.0f) * 1.25f * cameraScale),
			-1.0f, 1.0f);
		g_program.set_projection_matrix(g_projectionMatrix);
		g_program.set_view_matrix(g_viewMatrix);
	}

	if ((g_gameState.allEnemies.size() == 0) || (g_gameState.allPlayers.size() == 0)) {
		g_gameState.gameEndTimer += FIXED_TIMESTEP;
		//std::cout << g_gameState.gameEndTimer << std::endl;
	}
	if (g_gameState.gameEndTimer > 1.5f) {
		if (g_gameState.allEnemies.size() == 0) {
			g_gameState.victory = true;
			g_gameState.gameMode = GAME_OVER;
		}
		if (g_gameState.allPlayers.size() == 0) {
			g_gameState.victory = false;
			g_gameState.gameMode = GAME_OVER;
		}
	}
}

void updateGameOver() {

}

void render() {
	glClear(GL_COLOR_BUFFER_BIT);

	if (g_gameState.gameMode == GAME_LEVEL) {
		//std::string fuelText = "FUEL: " + std::to_string(g_gameState.fuel);
		//drawText(fuelText, glm::vec3(-(((float)fuelText.size() - 1) / 2.0f * 0.25f), 2.5f, 0.0f));
	}

	// Sort allEntities to Render from top of map first (for illusion of depth)
	std::vector<Entity*> sortedEntities;
	for (auto& entityPair : g_gameState.allEntities) {
		sortedEntities.push_back(&entityPair.second);
	}

	for (int i = 1; i < sortedEntities.size(); i++) {
		int j = i;
		while (j > 0 && sortedEntities[j]->getPosition().y > sortedEntities[j - 1]->getPosition().y) {
			std::swap(sortedEntities[j], sortedEntities[j - 1]);
			j--;
		}
	}

	/*
	for (float renderElevation = 0; renderElevation < 10; renderElevation += 0.1f) {
		for (auto& map : g_gameState.allLevelData[g_gameState.currentLevelIndex]) {
			if ((map.m_elevation >= renderElevation) && (map.m_elevation < renderElevation + 0.1f)) {
				map.render(&g_program);
			}
		}

		for (auto& entity : sortedEntities) {
			if ((entity->m_elevation >= renderElevation) && (entity->m_elevation < renderElevation + 0.1f)) {
				if (entity->m_isAlive) {
					entity->render(&g_program);
				}
			}
		}

		for (auto& effect : g_gameState.allEffects) {
			if ((effect.m_elevation >= renderElevation) && (effect.m_elevation < renderElevation + 0.1f)) {
				effect.render(&g_program);
			}
		}
	}
	*/

	// Now we have Renderable
	std::vector<Renderable*> sortedRenderable = { };
	for (auto& map : g_gameState.allLevelData[g_gameState.currentLevelIndex]) {
		sortedRenderable.push_back(&map);
	}
	for (auto& entity : sortedEntities) {
		if (entity->m_isAlive) {
			sortedRenderable.push_back(entity);
		}
	}
	for (auto& effect : g_gameState.allEffects) {
		sortedRenderable.push_back(&effect);
	}
	for (int i = 1; i < sortedRenderable.size(); i++) {
		int j = i;
		while (j > 0 && sortedRenderable[j]->m_elevation < sortedRenderable[j - 1]->m_elevation) {
			std::swap(sortedRenderable[j], sortedRenderable[j - 1]);
			j--;
		}
	}

	// Finally, render everything sorted
	for (auto& renderable : sortedRenderable) {
		renderable->render(&g_program);
	}

	if (g_gameState.gameMode == MAIN_MENU) {
		Util::drawText(g_program, "KILL ALL ENEMIES", glm::vec3(-((16.0f - 1) / 2.0f * 0.25f), 0.5f, 0.0f) + g_gameState.cameraPos, g_gameState.font.m_animCols, g_gameState.font.m_animRows, g_gameState.font.m_textureID);
		Util::drawText(g_program, "PRESS ANY KEY TO START", glm::vec3(-((22.0f - 1) / 2.0f * 0.25f), 0.25f, 0.0f) + g_gameState.cameraPos, g_gameState.font.m_animCols, g_gameState.font.m_animRows, g_gameState.font.m_textureID);
	}
	else if ((g_gameState.gameMode == GAME_OVER) && (g_gameState.victory)) {
		Util::drawText(g_program, "YOU WIN", glm::vec3(-((7.0f - 1) / 2.0f * 0.25f), 0.25f, 0.0f) + g_gameState.cameraPos, g_gameState.font.m_animCols, g_gameState.font.m_animRows, g_gameState.font.m_textureID);
		if (g_gameState.currentLevelIndex == g_gameState.allLevelData.size() - 1) {
			Util::drawText(g_program, "PRESS ANY KEY TO RESTART", glm::vec3(-((24.0f - 1) / 2.0f * 0.25f), 0.0f, 0.0f) + g_gameState.cameraPos, g_gameState.font.m_animCols, g_gameState.font.m_animRows, g_gameState.font.m_textureID);
		}
		else if(g_gameState.currentLevelIndex < g_gameState.allLevelData.size() - 1) {
			Util::drawText(g_program, "PRESS ANY KEY TO GO TO THE NEXT LEVEL", glm::vec3(-((37.0f - 1) / 2.0f * 0.25f), 0.0f, 0.0f) + g_gameState.cameraPos, g_gameState.font.m_animCols, g_gameState.font.m_animRows, g_gameState.font.m_textureID);
		}
	}
	else if ((g_gameState.gameMode == GAME_OVER) && (!g_gameState.victory)) {
		Util::drawText(g_program, "YOU DIED", glm::vec3(-((8.0f - 1) / 2.0f * 0.25f), 0.5f, 0.0f) + g_gameState.cameraPos, g_gameState.font.m_animCols, g_gameState.font.m_animRows, g_gameState.font.m_textureID);
		Util::drawText(g_program, "PRESS ANY KEY TO RESTART", glm::vec3(-((24.0f - 1) / 2.0f * 0.25f), 0.0f, 0.0f) + g_gameState.cameraPos, g_gameState.font.m_animCols, g_gameState.font.m_animRows, g_gameState.font.m_textureID);
	}

	SDL_GL_SwapWindow(g_displayWindow);
}

void shutdown() {
	g_gameState.allEntities.clear();
	g_gameState.allEffects.clear();
	g_gameState.allPlayers.clear();
	g_gameState.allEnemies.clear();
	SDL_Quit();
}
