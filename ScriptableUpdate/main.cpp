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
#include "ScriptableObject.h"
#include "ScriptableScript.h"
#include "Entity.h"
//#include "Effect.h"
//#include "GameMap.h"
#include "Util.h"
//#include "Level.h"
//#include "PlayerInput.h"
//#include "MovementController.h"

// ————— STRUCTS AND ENUMS —————//
enum GameMode { MAIN_MENU, GAME_LEVEL, GAME_OVER };

struct GameState {
	Entity font;
	GameMode gameMode = MAIN_MENU;
	bool victory = false;
	//std::map<std::string, ScriptableObject> allGameObjects = {};
	std::vector<ScriptableObject> allGameObjects = {};
	//std::map<std::string, Entity*> allEntities = {};
	//unsigned int currentLevelIndex = 0;
	//std::vector<Level> allLevel = {};
	//std::vector<std::vector<GameMap>> allLevelData = {};
	//std::map<std::string, Entity*> allEnemies = {};
	//std::map<std::string, Entity*> allRed = {};
	//std::map<std::string, Entity*> allPlayers = {};
	std::vector<unsigned int> entitiesGarbageCollection = {};
	//std::vector<ScriptableObject> allEffects = {};
	float gameEndTimer = 0.0f;
	glm::vec3 cameraPos = {0.0f, 0.0f, 0.0f};

	ScriptableObject* findGameObject(std::string objectID) {
		for (ScriptableObject gameObject : allGameObjects) {
			if (gameObject.getObjectID().find(objectID) != std::string::npos) {
				return &gameObject;
			}
		}
		return nullptr;
	}
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
	g_gameState.font = Entity(Util::loadTexture("Assets/FontAtlas2.png"), 5, 9, 0,
		{},
		0,
		0.0f, 0.0f, 0.0f,
		glm::vec3(1.0f, 1.0f, 1.0f)
	);

	// Map Grid Setup
	//GameMap(int width, int height, std::vector<unsigned int> levelData, std::vector<GLuint> tileTextureIDs, float tileSize, int tileCols, int tileRows);
	//Level::initialize(g_gameState.allLevelData);

	//allLevelData.push_back({});
	
	/*
	// Collision Layer 0
	
	ScriptableObject mapLevel = ScriptableObject("MapLevelCollisionLayer0", &g_gameState.allGameObjects);
	g_gameState.allGameObjects.push_back(mapLevel);
	GameMap* mapLevelMap = mapLevel.addComponent<GameMap>(new GameMap(true, true, 20, 12,
		0, {}, 0,
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		}, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Water/Water.png"), 1.0f, 1, 1, {}));
	mapLevelMap->setPosition(Z, 0.0f);

	// Collision Layer 1
	ScriptableObject mapLevel = ScriptableObject("MapLevelCollisionLayer1", &g_gameState.allGameObjects);
	g_gameState.allGameObjects.push_back(mapLevel);
	GameMap* mapLevelMap = mapLevel.addComponent<GameMap>(new GameMap(true, true, 20, 12,
		0, {}, 0,
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		}, Util::loadTexture("Assets/Empty.png"), 1.0f, 1, 1, {}));
	mapLevelMap->setPosition(Z, 0.0f);

	//Tile Edge Foam
	ScriptableObject mapLevel = ScriptableObject("MapLevelTileEdgeFoam", &g_gameState.allGameObjects);
	g_gameState.allGameObjects.push_back(mapLevel);
	GameMap* mapLevelMap = mapLevel.addComponent<GameMap>(new GameMap(false, false, 20, 12,
		0,
		{ { 0, { 0,  3,  6,  9, 12, 15, 18, 21, }},
		   { 1, { 1,  4,  7, 10, 13, 16, 19, 22, }},
		   { 2, { 2,  5,  8, 11, 14, 17, 20, 23, }},
		   {24, {24, 27, 30, 33, 36, 39, 42, 45, }},
		   {25, {25, 28, 31, 34, 37, 40, 43, 46, }},
		   {26, {26, 29, 32, 35, 38, 41, 44, 47, }},
		   {48, {48, 51, 54, 57, 60, 63, 66, 69, }},
		   {49, {49, 52, 55, 58, 61, 64, 67, 70, }},
		   {50, {50, 53, 56, 59, 62, 65, 68, 71, }},

		}, 0,
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		}, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Water/Foam/Foam.png"), 1.0f, 24, 3,
		{ {"TL",  0 }, // Top Left
			{"TC",  1 }, // Top Center
			{"TR",  2 }, // Top Right
			{"ML", 24 }, // Middle Left
			{"MC", 25 }, // Middle Center
			{"MR", 26 }, // Middle Right
			{"BL", 48 }, // Bottom Left
			{"BC", 49 }, // Bottom Center
			{"BR", 50 }, // Bottom Right
			{"VT",  1 }, // Vertical Top
			{"VM", 25 }, // Vertical Middle
			{"VB", 49 }, // Vertical Bottom
			{"HL", 48 }, // Horizontal Left
			{"HC", 49 }, // Horizontal Center
			{"HR", 50 }, // Horizontal Right
			{"[]", 49 }, // Isolated
		}));
	mapLevelMap->setPosition(Z, 0.0f);

	// Elevation Wall
	ScriptableObject mapLevel = ScriptableObject("MapLevelElevationWall", &g_gameState.allGameObjects);
	g_gameState.allGameObjects.push_back(mapLevel);
	GameMap* mapLevelMap = mapLevel.addComponent<GameMap>(new GameMap(false, false, 20, 12,
		0, {}, 0,
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		}, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Ground/Tilemap_Elevation.png"), 1.0f, 4, 8,
		{ {"TL", 12 }, // Top Left
			{"TC", 13 }, // Top Center
			{"TR", 14 }, // Top Right
			{"ML", 12 }, // Middle Left
			{"MC", 13 }, // Middle Center
			{"MR", 14 }, // Middle Right
			{"BL", 12 }, // Bottom Left
			{"BC", 13 }, // Bottom Center
			{"BR", 14 }, // Bottom Right
			{"VT", 15 }, // Vertical Top
			{"VM", 15 }, // Vertical Middle
			{"VB", 15 }, // Vertical Bottom
			{"HL", 12 }, // Horizontal Left
			{"HC", 13 }, // Horizontal Center
			{"HR", 14 }, // Horizontal Right
			{"[]", 15 }, // Isolated
		}));
	mapLevelMap->setPosition(Z, 0.0f);

	// Elevation Ground
	ScriptableObject mapLevel = ScriptableObject("MapLevelElevationWall", &g_gameState.allGameObjects);
	g_gameState.allGameObjects.push_back(mapLevel);
	GameMap* mapLevelMap = mapLevel.addComponent<GameMap>(new GameMap(false, false, 20, 12,
		0, {}, 0,
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		}, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Ground/Tilemap_Elevation.png"), 1.0f, 4, 8,
		{ {"TL",  0 }, // Top Left
			{"TC",  1 }, // Top Center
			{"TR",  2 }, // Top Right
			{"ML",  4 }, // Middle Left
			{"MC",  5 }, // Middle Center
			{"MR",  6 }, // Middle Right
			{"BL",  8 }, // Bottom Left
			{"BC",  9 }, // Bottom Center
			{"BR", 10 }, // Bottom Right
			{"VT",  3 }, // Vertical Top
			{"VM",  7 }, // Vertical Middle
			{"VB", 11 }, // Vertical Bottom
			{"HL", 16 }, // Horizontal Left
			{"HC", 17 }, // Horizontal Center
			{"HR", 18 }, // Horizontal Right
			{"[]", 19 }, // Isolated
		}));
	mapLevelMap->setPosition(Z, 1.0f);

	// Elevation Grass Green
	ScriptableObject mapLevel = ScriptableObject("MapLevelElevationWall", &g_gameState.allGameObjects);
	g_gameState.allGameObjects.push_back(mapLevel);
	GameMap* mapLevelMap = mapLevel.addComponent<GameMap>(new GameMap(false, false, 20, 12,
		0, {}, 0,
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		}, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Ground/Tilemap_Flat.png"), 1.0f, 10, 4,
		{ {"TL",  0 }, // Top Left
			{"TC",  1 }, // Top Center
			{"TR",  2 }, // Top Right
			{"ML", 10 }, // Middle Left
			{"MC", 11 }, // Middle Center
			{"MR", 12 }, // Middle Right
			{"BL", 20 }, // Bottom Left
			{"BC", 21 }, // Bottom Center
			{"BR", 22 }, // Bottom Right
			{"VT",  3 }, // Vertical Top
			{"VM", 13 }, // Vertical Middle
			{"VB", 23 }, // Vertical Bottom
			{"HL", 30 }, // Horizontal Left
			{"HC", 31 }, // Horizontal Center
			{"HR", 32 }, // Horizontal Right
			{"[]", 33 }, // Isolated
		}));
	mapLevelMap->setPosition(Z, 1.0f);

	//g_gameState.currentLevelIndex = 0;
	*/

	// Reset Game
	resetGame();
}

void resetGame() {
	g_gameState.gameEndTimer = 0.0f;
	g_gameState.victory = false;
	g_gameState.allGameObjects.clear();
	
	srand(time(NULL)); // Reset Random

	// Initialize Objects (models and assets)

	// Player
	ScriptableObject player = ScriptableObject("Player", &g_gameState.allGameObjects);
	g_gameState.allGameObjects.push_back(player);
	Entity* playerEntity = player.addComponent<Entity>(
		new Entity(Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Warrior/Blue/Warrior_Blue.png"), 6, 8, 0,
			{ { 0,  1,  2,  3,  4,  5},
				{ 6,  7,  8,  9, 10, 11},
				{12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
				{24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
				{36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
			},
			0, 5, 2, 2.5f,
			glm::vec3(0.5f, 0.5f, 1.0f)
		));
	playerEntity->setPosition(glm::vec3(3.5f, -5.5f, 1.0f));
	playerEntity->update(FIXED_TIMESTEP);
	//player.addComponent<PlayerInput>();
	//player.addComponent<MovementController>();

	// Move Camera to Player
	g_gameState.cameraPos = glm::vec3(g_gameState.findGameObject("Player")->getPosition().x, g_gameState.findGameObject("Player")->getPosition().y, 0.0f);
	g_viewMatrix = glm::mat4(1.0f);
	g_viewMatrix = glm::translate(g_viewMatrix, glm::vec3(-g_gameState.cameraPos.x, -g_gameState.cameraPos.y, g_gameState.cameraPos.z));
	g_program.set_view_matrix(g_viewMatrix);
	
	g_gameState.gameMode = GAME_LEVEL;
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
	
	/*
	for (auto& object : g_gameState.allGameObjects) {
		if (PlayerInput* playerInput = object.getComponent<PlayerInput>()) {
			playerInput->processInput();
		}
	}
	*/
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

	/*
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
	*/

	// Clean up entities


	/*
	for (unsigned int i = 0; i < g_gameState.allGameObjects.size(); i++) {
		if (Entity * objectEntity = g_gameState.allGameObjects[i].getComponent<Entity>()) {
			if (objectEntity->getAlive()) {
				g_gameState.entitiesGarbageCollection.push_back(i);
				// Remember to clean from Collisions
			}
		}
	}

	for (unsigned int i : g_gameState.entitiesGarbageCollection) {
		g_gameState.allGameObjects.erase(g_gameState.allGameObjects.begin() + i);
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
	for (auto& gameObject : g_gameState.allGameObjects) {
		if (Entity* gameObjectEntity = gameObject.getComponent<Entity>()) {
			if (gameObjectEntity->getAlive()) {
				gameObjectEntity->update(FIXED_TIMESTEP);
			}
		}
		//else if (GameMap* gameObjectGameMap = gameObject.getComponent<GameMap>()) {
		//	gameObjectGameMap->update(FIXED_TIMESTEP);
		//}
	}

	/*
	for (auto& effect : g_gameState.allEffects) {
		effect.update(FIXED_TIMESTEP);
	}
	*/

	// Move Camera to Player
	if (ScriptableObject* player = g_gameState.findGameObject("Player")) {
		//std::cout << "Player alive? " << g_gameState.allEntities["Player"].getPosition().x << ", " << g_gameState.allEntities["Player"].getPosition().y << std::endl;
		g_gameState.cameraPos = glm::vec3(player->getPosition().x, player->getPosition().y, 0.0f);
		g_viewMatrix = glm::mat4(1.0f);
		g_viewMatrix = glm::translate(g_viewMatrix, glm::vec3(-g_gameState.cameraPos.x, -g_gameState.cameraPos.y, g_gameState.cameraPos.z));

		g_projectionMatrix = glm::ortho(-((WINDOW_WIDTH / 320.0f) * 1.25f * cameraScale), ((WINDOW_WIDTH / 320.0f) * 1.25f * cameraScale),
			-((WINDOW_HEIGHT / 320.0f) * 1.25f * cameraScale), ((WINDOW_HEIGHT / 320.0f) * 1.25f * cameraScale),
			-1.0f, 1.0f);
		g_program.set_projection_matrix(g_projectionMatrix);
		g_program.set_view_matrix(g_viewMatrix);
	}

	/*
	if ((!g_gameState.findGameObject("Player")) || (!g_gameState.findGameObject("Enemy"))) {
		g_gameState.gameEndTimer += FIXED_TIMESTEP;
		//std::cout << g_gameState.gameEndTimer << std::endl;
	}
	if (g_gameState.gameEndTimer > 1.5f) {
		if (!g_gameState.findGameObject("Enemy")) {
			g_gameState.victory = true;
			g_gameState.gameMode = GAME_OVER;
		}
		if (!g_gameState.findGameObject("Player")) {
			g_gameState.victory = false;
			g_gameState.gameMode = GAME_OVER;
		}
	}
	*/
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
	std::vector<ScriptableObject*> sortedRender = { };
	for (auto& gameObject : g_gameState.allGameObjects) {
		sortedRender.push_back(&gameObject);
	}

	// Does map render over entities?
	for (int i = 1; i < sortedRender.size(); i++) {
		int j = i;
		while (j > 0 && ((sortedRender[j]->getPosition().z < sortedRender[j - 1]->getPosition().z) || (sortedRender[j]->getPosition().y > sortedRender[j - 1]->getPosition().y))) {
			std::swap(sortedRender[j], sortedRender[j - 1]);
			j--;
		}
	}

	// Finally, render everything sorted
	for (auto& renderable : sortedRender) {
		renderable->render(&g_program);
	}

	if (g_gameState.gameMode == MAIN_MENU) {
		Util::drawText(g_program, "KILL ALL ENEMIES", glm::vec3(-((16.0f - 1) / 2.0f * 0.25f), 0.5f, 0.0f) + g_gameState.cameraPos, g_gameState.font.m_animCols, g_gameState.font.m_animRows, g_gameState.font.m_textureID);
		Util::drawText(g_program, "PRESS ANY KEY TO START", glm::vec3(-((22.0f - 1) / 2.0f * 0.25f), 0.25f, 0.0f) + g_gameState.cameraPos, g_gameState.font.m_animCols, g_gameState.font.m_animRows, g_gameState.font.m_textureID);
	}
	else if ((g_gameState.gameMode == GAME_OVER) && (g_gameState.victory)) {
		Util::drawText(g_program, "YOU WIN", glm::vec3(-((7.0f - 1) / 2.0f * 0.25f), 0.25f, 0.0f) + g_gameState.cameraPos, g_gameState.font.m_animCols, g_gameState.font.m_animRows, g_gameState.font.m_textureID);
		/*
		if (g_gameState.currentLevelIndex == g_gameState.allLevelData.size() - 1) {
			Util::drawText(g_program, "PRESS ANY KEY TO RESTART", glm::vec3(-((24.0f - 1) / 2.0f * 0.25f), 0.0f, 0.0f) + g_gameState.cameraPos, g_gameState.font.m_animCols, g_gameState.font.m_animRows, g_gameState.font.m_textureID);
		}
		else if(g_gameState.currentLevelIndex < g_gameState.allLevelData.size() - 1) {
			Util::drawText(g_program, "PRESS ANY KEY TO GO TO THE NEXT LEVEL", glm::vec3(-((37.0f - 1) / 2.0f * 0.25f), 0.0f, 0.0f) + g_gameState.cameraPos, g_gameState.font.m_animCols, g_gameState.font.m_animRows, g_gameState.font.m_textureID);
		}
		*/
	}
	else if ((g_gameState.gameMode == GAME_OVER) && (!g_gameState.victory)) {
		Util::drawText(g_program, "YOU DIED", glm::vec3(-((8.0f - 1) / 2.0f * 0.25f), 0.5f, 0.0f) + g_gameState.cameraPos, g_gameState.font.m_animCols, g_gameState.font.m_animRows, g_gameState.font.m_textureID);
		Util::drawText(g_program, "PRESS ANY KEY TO RESTART", glm::vec3(-((24.0f - 1) / 2.0f * 0.25f), 0.0f, 0.0f) + g_gameState.cameraPos, g_gameState.font.m_animCols, g_gameState.font.m_animRows, g_gameState.font.m_textureID);
	}

	SDL_GL_SwapWindow(g_displayWindow);
}

void shutdown() {
	g_gameState.allGameObjects.clear();
	SDL_Quit();
}
