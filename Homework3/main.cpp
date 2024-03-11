/**
* Author: John Li
* Assignment: Lunar Lander
* Date due: 2024-09-03, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#define FIXED_TIMESTEP 0.0166666f
#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"
#include <vector>
#include <ctime>
#include "Entity.h"


// ————— STRUCTS AND ENUMS —————//
enum GameMode { MAIN_MENU, GAME_LEVEL, GAME_OVER };

struct GameState
{
	Entity font;
	GameMode gameMode = MAIN_MENU;
	bool victory = false;
	std::map<std::string, Entity> allEntities = {};
	std::vector<std::vector<bool>> tileGrid = {};
	std::map<std::string, Entity*> allSolid = {};
	std::map<std::string, Entity*> allCoin = {};
	std::string grabbedCoin = "0";
	int fuel = 9999;
};

// Window
const int WINDOW_WIDTH = 640 * 2;
const int WINDOW_HEIGHT = 480 * 2;

// BG
float g_bgR = 0.0f;
float g_bgG = 0.0f;
float g_bgB = 0.0f;
float g_bgA = 1.0f;

// Camera
const int VIEWPORT_X = 0;
const int VIEWPORT_Y = 0;
const int VIEWPORT_WIDTH = WINDOW_WIDTH;
const int VIEWPORT_HEIGHT = WINDOW_HEIGHT;

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

// Image Globals
const int NUMBER_OF_TEXTURES = 1; // Number of textures generated per model
const GLint LEVEL_OF_DETAIL = 0; // base image level
const GLint TEXTURE_BORDER = 0;

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
GLuint loadTexture(const char* filepath);
void drawText(std::string text, glm::vec3 position);
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
	g_displayWindow = SDL_CreateWindow("Hello, Grabber!",
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
	g_projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);  // Defines the characteristics of your camera, such as clip planes, field of view, projection method etc.

	g_program.set_projection_matrix(g_projectionMatrix);
	g_program.set_view_matrix(g_viewMatrix);

	glUseProgram(g_program.get_program_id());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(g_bgR, g_bgG, g_bgB, g_bgA);

	// Initialize Game State

	// Font Texture
	g_gameState.font = Entity("Font", loadTexture("Assets/FontAtlas.png"), 9, 5, 0,
		{},
		0,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		0.0f,
		glm::vec3(1.0f, 1.0f, 1.0f)
	);

	// Reset Game
	resetGame();
}

void resetGame() {
	g_gameState.victory = false;
	g_gameState.allSolid.clear();
	g_gameState.allCoin.clear();
	g_gameState.tileGrid.clear();
	g_gameState.allEntities.clear();
	g_gameState.grabbedCoin = "0";
	g_gameState.fuel = 9999;

	for (int y = 0; y < 4; y++) {
		g_gameState.tileGrid.push_back({});
		for (int x = 0; x < 10; x++) {
			g_gameState.tileGrid[y].push_back(true);
		}
	}

	g_gameState.tileGrid.push_back({});
	for (int x = 0; x < 10; x++) {
		g_gameState.tileGrid[4].push_back(false);
	}

	for (int y = 4; y >= 0; y--) {
		//std::cout << "{ ";
		for (int x = 0; x < 10; x++) {
			std::cout << g_gameState.tileGrid[y][x] << " ";
		}
		//std::cout << "}\n";
	}
	//std::cout << "\n";

	srand(time(NULL));
	// Create Uneven Surface
	for (int _ = 0; _ < 4; _++) {
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 10; x++) {
				if (!g_gameState.tileGrid[y + 1][x]) {
					if ((rand() % 100) < 50) {
						//std::cout << "chance! " << y << std::endl;
						if ((y + 2 > g_gameState.tileGrid.size() - 1) || !(((x + 1 < g_gameState.tileGrid[y + 2].size()) && g_gameState.tileGrid[y + 2][x + 1]) || ((x - 1 >= 0) && g_gameState.tileGrid[y + 2][x - 1]))) {
							//std::cout << "set false {" << x << ", " << y << "}" << std::endl;
							g_gameState.tileGrid[y][x] = false;
						}
					}
				}
			}
		}
	}

	for (int y = 4; y >= 0; y--) {
		std::cout << "{ ";
		for (int x = 0; x < 10; x++) {
			std::cout << g_gameState.tileGrid[y][x] << " ";
		}
		std::cout << "}\n";
	}
	std::cout << "\n";

	// Initialize Objects (models and assets)

	// Player
	g_gameState.allEntities["GrabberHand"] = Entity("GrabberHand", loadTexture("Assets/GrabberHand.png"), 0, 0, 0,
		{},
		0,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		5.0f,
		glm::vec3(0.85f, 0.85f, 1.0f)
	);
	g_gameState.allEntities["GrabberHand"].setPosition(glm::vec3(0.0f));
	g_gameState.allEntities["GrabberHand"].setMovement(glm::vec3(0.0f));
	g_gameState.allEntities["GrabberHand"].setAcceleration(glm::vec3(0.0f, ACC_OF_GRAVITY, 0.0f));
	g_gameState.allEntities["GrabberHand"].setPosition(glm::vec3(0.0f, 4.0f, 0.0f));
	g_gameState.allEntities["GrabberHand"].update(g_deltaTime);

	// Ground
	std::string wallName = "Ground";
	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 10; x++) {
			if (g_gameState.tileGrid[y][x]) {
				// Tiling
				int textureIndex = 15;
				if ((((y + 1) < g_gameState.tileGrid.size()) && ((y - 1) >= 0) && ((x + 1) < g_gameState.tileGrid[y].size()) && ((x - 1) >= 0)) &&
					(g_gameState.tileGrid[y + 1][x] && g_gameState.tileGrid[y - 1][x] && g_gameState.tileGrid[y][x + 1] && g_gameState.tileGrid[y][x - 1])) {
					textureIndex = 5;
				}
				else if ((((y + 1) < g_gameState.tileGrid.size()) && ((y - 1) >= 0) && ((x + 1) < g_gameState.tileGrid[y].size())) &&
					(g_gameState.tileGrid[y + 1][x] && g_gameState.tileGrid[y - 1][x] && g_gameState.tileGrid[y][x + 1])) {
					textureIndex = 4;
				}
				else if ((((y + 1) < g_gameState.tileGrid.size()) && ((y - 1) >= 0) && ((x - 1) >= 0)) &&
					(g_gameState.tileGrid[y + 1][x] && g_gameState.tileGrid[y - 1][x] && g_gameState.tileGrid[y][x - 1])) {
					textureIndex = 6;
				}
				else if ((((y - 1) >= 0) && ((x + 1) < g_gameState.tileGrid[y].size()) && ((x - 1) >= 0)) &&
					(g_gameState.tileGrid[y - 1][x] && g_gameState.tileGrid[y][x + 1] && g_gameState.tileGrid[y][x - 1])) {
					textureIndex = 1;
				}
				else if ((((y - 1) >= 0) && ((x + 1) < g_gameState.tileGrid[y].size())) &&
					(g_gameState.tileGrid[y - 1][x] && g_gameState.tileGrid[y][x + 1])) {
					textureIndex = 0;
				}
				else if ((((y - 1) >= 0) && ((x - 1) >= 0)) &&
					(g_gameState.tileGrid[y - 1][x] && g_gameState.tileGrid[y][x - 1])) {
					textureIndex = 2;
				}
				else if ((((y + 1) < g_gameState.tileGrid.size()) && ((x + 1) < g_gameState.tileGrid[y].size()) && ((x - 1) >= 0)) &&
					(g_gameState.tileGrid[y + 1][x] && g_gameState.tileGrid[y][x + 1] && g_gameState.tileGrid[y][x - 1])) {
					textureIndex = 9;
				}
				else if ((((y + 1) < g_gameState.tileGrid.size()) && ((x + 1) < g_gameState.tileGrid[y].size())) &&
					(g_gameState.tileGrid[y + 1][x] && g_gameState.tileGrid[y][x + 1])) {
					textureIndex = 8;
				}
				else if ((((y + 1) < g_gameState.tileGrid.size()) && ((x - 1) >= 0)) &&
					(g_gameState.tileGrid[y + 1][x] && g_gameState.tileGrid[y][x - 1])) {
					textureIndex = 10;
				}
				else if ((((y + 1) < g_gameState.tileGrid.size()) && ((y - 1) >= 0)) &&
					(g_gameState.tileGrid[y + 1][x] && g_gameState.tileGrid[y - 1][x])) {
					textureIndex = 7;
				}
				else if (((y - 1) >= 0) &&
					(g_gameState.tileGrid[y - 1][x])) {
					textureIndex = 3;
				}
				else if (((y + 1) < g_gameState.tileGrid.size()) &&
					(g_gameState.tileGrid[y + 1][x])) {
					textureIndex = 11;
				}
				else if (((((x + 1) < g_gameState.tileGrid[y].size()) && ((x - 1) >= 0))) &&
					(g_gameState.tileGrid[y][x + 1] && g_gameState.tileGrid[y][x - 1])) {
					textureIndex = 13;
				}
				else if (((x + 1) < g_gameState.tileGrid[y].size()) &&
					(g_gameState.tileGrid[y][x + 1])) {
					textureIndex = 12;
				}
				else if (((x - 1) >= 0) &&
					(g_gameState.tileGrid[y][x - 1])) {
					textureIndex = 14;
				}
				else {
					textureIndex = 15;
				}

				std::string tempWallName = wallName + std::to_string(x) + std::to_string(y);
				g_gameState.allEntities[tempWallName] = Entity(tempWallName, loadTexture("Assets/GroundAtlas.png"), 4, 4, textureIndex,
					{ },
					0,
					glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.0f, 0.0f, 0.0f),
					0.0f,
					glm::vec3(.78f, 1.0f, 1.0f)
				);
				g_gameState.allEntities[tempWallName].setPosition(glm::vec3(-4.5f + x, -4.0f + y, 0.0f));
				g_gameState.allEntities[tempWallName].update(g_deltaTime);
				g_gameState.allSolid[tempWallName] = &g_gameState.allEntities[tempWallName];
			}
		}
		g_gameState.gameMode = MAIN_MENU;
	}

	// Coin
	std::string coinName = "Coin";
	std::vector<std::vector<int>> topTile;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 10; x++) {
			if (g_gameState.tileGrid[y][x] && !g_gameState.tileGrid[y + 1][x]) {
				topTile.push_back({ x, y });
			}
		}
	}

	for (int i = 0; i < 3; i++) {
		unsigned int chosenIndex = rand() % topTile.size();
		std::vector<int> chosen = topTile[chosenIndex];

		std::string tempCoinName = coinName + std::to_string(i);
		std::cout << tempCoinName << std::endl;
		g_gameState.allEntities[tempCoinName] = Entity(tempCoinName, loadTexture("Assets/CoinAtlas.png"), 1, 3, i,
			{ },
			0,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			0.0f,
			glm::vec3(.01f, 0.01f, 1.0f)
		);
		g_gameState.allEntities[tempCoinName].setPosition(glm::vec3(-4.5f + chosen[0], -4.0f + chosen[1] + 1.0f, 0.0f));
		g_gameState.allEntities[tempCoinName].update(g_deltaTime);
		g_gameState.tileGrid[chosen[1]][chosen[0]] = true;
		topTile.erase(topTile.begin() + chosenIndex);
		g_gameState.allCoin[tempCoinName] = &g_gameState.allEntities[tempCoinName];
	}
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
		}
	}

	// Hold
	const Uint8* key_state = SDL_GetKeyboardState(NULL);

	if ((key_state[SDL_SCANCODE_UP]) && (key_state[SDL_SCANCODE_DOWN]))
	{
		// Nothing
	}
	else if (key_state[SDL_SCANCODE_UP])
	{
		if (g_gameState.fuel > 0) {
			g_gameState.allEntities["GrabberHand"].setAcceleration(
				glm::vec3(
					g_gameState.allEntities["GrabberHand"].getAcceleration().x,
					g_gameState.allEntities["GrabberHand"].getAcceleration().y + (1.0f * FIXED_TIMESTEP),
					g_gameState.allEntities["GrabberHand"].getAcceleration().z
				)
			);
			g_gameState.fuel -= 1;
			if (g_gameState.fuel < 0) {
				g_gameState.fuel = 0;
			}
		}
		else if (g_gameState.fuel < 0) {
			g_gameState.fuel = 0;
		}
	}
	else if (key_state[SDL_SCANCODE_DOWN])
	{
		// Nothing
	}
	else
	{
		// Nothing
	}

	if ((key_state[SDL_SCANCODE_LEFT]) && (key_state[SDL_SCANCODE_RIGHT]))
	{
		// Nothing
	}
	else if (key_state[SDL_SCANCODE_LEFT])
	{
		if (g_gameState.fuel > 0) {
			g_gameState.allEntities["GrabberHand"].setAcceleration(
				glm::vec3(
					g_gameState.allEntities["GrabberHand"].getAcceleration().x - (2.5f * FIXED_TIMESTEP),
					g_gameState.allEntities["GrabberHand"].getAcceleration().y,
					g_gameState.allEntities["GrabberHand"].getAcceleration().z
				)
			);
			g_gameState.fuel -= 1;
			if (g_gameState.fuel < 0) {
				g_gameState.fuel = 0;
			}
		}
		else if (g_gameState.fuel < 0) {
			g_gameState.fuel = 0;
		}
	}
	else if (key_state[SDL_SCANCODE_RIGHT])
	{
		if (g_gameState.fuel > 0) {
			g_gameState.allEntities["GrabberHand"].setAcceleration(
				glm::vec3(
					g_gameState.allEntities["GrabberHand"].getAcceleration().x + (2.5f * FIXED_TIMESTEP),
					g_gameState.allEntities["GrabberHand"].getAcceleration().y,
					g_gameState.allEntities["GrabberHand"].getAcceleration().z
				)
			);
			g_gameState.fuel -= 1;
			if (g_gameState.fuel < 0) {
				g_gameState.fuel = 0;
			}
		}
	else if (g_gameState.fuel < 0) {
		g_gameState.fuel = 0;
	}
	}
	else
	{
		// Nothing
	}

	// Normalization
	if (glm::length(g_gameState.allEntities["GrabberHand"].getAcceleration()) > 1.0f)
	{
		g_gameState.allEntities["GrabberHand"].setAcceleration(glm::normalize(g_gameState.allEntities["GrabberHand"].getAcceleration()));
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
	if (g_deltaTime < FIXED_TIMESTEP)
	{
		g_timeAccumulator = g_deltaTime;
		return;
	}

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
		if (entityPair.first == "GrabberHand") {
			entityPair.second.checkCollision(FIXED_TIMESTEP, g_gameState.allSolid);
			if (entityPair.second.getAcceleration().y > -2.0f) {
				entityPair.second.setAcceleration(glm::vec3(entityPair.second.getAcceleration().x, entityPair.second.getAcceleration().y - (0.5f * FIXED_TIMESTEP), entityPair.second.getAcceleration().z));
				//entityPair.second.setAcceleration(entityPair.second.getAcceleration() + glm::vec3(0.0f, -0.5f * FIXED_TIMESTEP, 0.0f));
			}

			// Air Friction
			if (entityPair.second.getVelocity().y < -6.0f) {
				entityPair.second.setVelocity(glm::vec3(entityPair.second.getVelocity().x, -6.0f, entityPair.second.getVelocity().z));
			}

			// Weight
			if (entityPair.second.getVelocity().y > 0.2f) {
				entityPair.second.setVelocity(glm::vec3(entityPair.second.getVelocity().x, 0.2f, entityPair.second.getVelocity().z));
			}

			//std::cout << entityPair.second.getVelocity().y << " ";
			//std::cout << entityPair.second.getAcceleration().y << "\n";

			for (auto coin : g_gameState.allCoin) {
				if (entityPair.second.isCollidingWith(coin.second)) {
					if (g_gameState.victory == false) {
						g_gameState.grabbedCoin = std::to_string(coin.second->m_animIndex + coin.second->m_animIndex + 1);
						g_gameState.victory = true;
						g_gameState.gameMode = GAME_OVER;
					}
				}
			}

			if (entityPair.second.m_collidingWith.size() > 0) {
				g_gameState.victory = false;
				g_gameState.gameMode = GAME_OVER;
			}
		}
		entityPair.second.update(FIXED_TIMESTEP);
	}
}

void updateGameOver() {

}

void render() {
	glClear(GL_COLOR_BUFFER_BIT);

	if (g_gameState.gameMode == GAME_LEVEL) {
		std::string fuelText = "FUEL: " + std::to_string(g_gameState.fuel);
		drawText(fuelText, glm::vec3(-(((float)fuelText.size() - 1) / 2.0f * 0.25f), 2.5f, 0.0f));
	}

	for (auto& entityPair : g_gameState.allEntities) {
		entityPair.second.render(&g_program);
	}

	if (g_gameState.gameMode == MAIN_MENU) {
		drawText("GRAB A COIN", glm::vec3(-1.25f, 0.5f, 0.0f));
		drawText("PRESS ANY KEY TO START", glm::vec3(-((22.0f - 1) / 2.0f * 0.25f), 0.25f, 0.0f));
	}
	else if ((g_gameState.gameMode == GAME_OVER) && (g_gameState.victory)) {
		drawText("GRABBED THE " + g_gameState.grabbedCoin + " COIN", glm::vec3(-((18.0f - 1) / 2.0f * 0.25f), 0.5f, 0.0f));
		drawText("YOU WIN", glm::vec3(-((7.0f - 1) / 2.0f * 0.25f), 0.25f, 0.0f));
		drawText("PRESS ANY KEY TO RESTART", glm::vec3(-((24.0f - 1) / 2.0f * 0.25f), 0.0f, 0.0f));
	}
	else if ((g_gameState.gameMode == GAME_OVER) && (!g_gameState.victory)) {
		drawText("MISSED", glm::vec3(-((6.0f - 1) / 2.0f * 0.25f), 0.5f, 0.0f));
		drawText("YOU LOSE", glm::vec3(-((8.0f - 1) / 2.0f * 0.25f), 0.25f, 0.0f));
		drawText("PRESS ANY KEY TO RESTART", glm::vec3(-((24.0f - 1) / 2.0f * 0.25f), 0.0f, 0.0f));
	}

	SDL_GL_SwapWindow(g_displayWindow);
}

GLuint loadTexture(const char* filepath) {
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

void drawText(std::string text, glm::vec3 position) {
	float width = 1.0f / g_gameState.font.m_animCols;
	float height = 1.0f / g_gameState.font.m_animRows;

	std::vector<float> vertices;
	std::vector<float> textureCoordinates;

	float screenSize = 0.25f;
	float spacing = 0.01f;

	for (int i = 0; i < text.size(); i++) {
		int index = (int)text[i] - 48;  // ascii value of character
		float offset = (screenSize + spacing) * i;

		float uCoord = (float)(index % g_gameState.font.m_animCols) / (float)g_gameState.font.m_animCols;
		float vCoord = (float)(index / g_gameState.font.m_animCols) / (float)g_gameState.font.m_animRows;

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
	
	g_program.set_model_matrix(model_matrix);
	glUseProgram(g_program.get_program_id());

	glVertexAttribPointer(g_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices.data());
	glEnableVertexAttribArray(g_program.get_position_attribute());
	glVertexAttribPointer(g_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, textureCoordinates.data());
	glEnableVertexAttribArray(g_program.get_tex_coordinate_attribute());

	glBindTexture(GL_TEXTURE_2D, g_gameState.font.m_textureID);
	glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

	glDisableVertexAttribArray(g_program.get_position_attribute());
	glDisableVertexAttribArray(g_program.get_tex_coordinate_attribute());
}

void shutdown() {
	SDL_Quit();
}
	