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
//#include "Entity.h"
#include "Util.h"
#include "GameObject.h"
#include "Component.h"
#include "SpriteRenderer.h"
#include "Rigidbody2D.h"
#include "PlayerMovementController.h"
#include "Collider2D.h"
#include "PolygonCollider2D.h"
#include "CircleCollider2D.h"
#include "TileMap.h"
#include "Scene.h"
#include "Scene1.h"

// ————— STRUCTS AND ENUMS —————//
enum GameMode { MAIN_MENU, GAME_LEVEL, GAME_OVER };

/*
struct GameState {
	//Entity* font;
	GameMode gameMode = GAME_LEVEL;
	bool victory = false;
	std::vector<GameObject*> allGameObjects = {};
	glm::vec3 cameraPos = {0.0f, 0.0f, 0.0f};

	GameObject* findGameObject(std::string objectID) {
		for (GameObject* gameObject : allGameObjects) {
			if (gameObject->getObjectID().find(objectID) != std::string::npos) {
				return gameObject;
			}
		}
		return nullptr;
	}
};
*/

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
Scene* g_gameScene;
std::vector<Scene*> g_gameScenes;

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

void switchScene(int sceneInd ) {
	g_gameScene = g_gameScenes[sceneInd];
	g_gameScene->initialise();
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

	// Font Texture
	//g_gameState.font = new Entity(Util::loadTexture("Assets/FontAtlas2.png"), 5, 9, 0,
	//	{},
	//	0,
	//	0.0f, 0.0f, 0.0f,
	//	glm::vec3(1.0f, 1.0f, 1.0f)
	//);

	// Map Grid Setup

	g_gameScenes.push_back(new Scene1());
	switchScene(0);

	// Reset Game
	resetGame();

}

void resetGame() {
	g_gameState.victory = false;

	for (auto gameObject : g_gameState.allGameObjects)
	{
		delete gameObject;
	}
	g_gameState.allGameObjects.clear();

	srand(time(NULL)); // Reset Random

	// Initialize Objects (models and assets)

	
	// Reference
	GameObject* reference = new GameObject("Reference", &g_gameState.allGameObjects);
	g_gameState.allGameObjects.push_back(reference);
	reference->addComponent<SpriteRenderer>(
		new SpriteRenderer(reference, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Warrior/Blue/Warrior_Blue.png"), 6, 8, 0,
			{ { 0,  1,  2,  3,  4,  5},
				{ 6,  7,  8,  9, 10, 11},
				{12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
				{24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
				{36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
			},
			0, 12));

	reference->setPosition(glm::vec3(3.5f, 0.0f, 1.0f));
	reference->update(FIXED_TIMESTEP);
	reference->addComponent<Rigidbody2D>();
	
	reference->addComponent<PolygonCollider2D>(
		new PolygonCollider2D(reference,
			{   {-0.5f,  0.5f, 0.0f},
				{ 0.5f,  0.5f, 0.0f},
				{ 0.5f, -0.5f, 0.0f},
				{-0.5f, -0.5f, 0.0f}, }))->setColliderScale({ 0.4f, 0.4f, 1.0f });
	//reference->addComponent<CircleCollider2D>(new CircleCollider2D(reference, { 1.0f, 1.0f, 1.0f }));
	
	/*
	// Ellipse
	GameObject* ellipse = new GameObject("Ellipse", &g_gameState.allGameObjects);
	g_gameState.allGameObjects.push_back(ellipse);
	ellipse->addComponent<SpriteRenderer>(
		new SpriteRenderer(ellipse, Util::loadTexture("Assets/Ellipse.png"), 1, 1, 0,
			{ }, 0, 0));

	ellipse->setPosition(glm::vec3(6.5f, -2.0f, 1.0f));
	ellipse->setScale({ 1.0f, 2.0f, 1.0f });
	ellipse->setRotation(Z, 0.0f);
	ellipse->update(FIXED_TIMESTEP);
	ellipse->addComponent<Rigidbody2D>()->setRBType(STATIC);
	ellipse->addComponent<CircleCollider2D>(new CircleCollider2D(ellipse, { 0.5f, 0.5f, 0.0f }));
	*/

	/*
	// Ellipse 2
	GameObject* ellipse2 = new GameObject("Ellipse 2", &g_gameState.allGameObjects);
	g_gameState.allGameObjects.push_back(ellipse2);
	ellipse2->addComponent<SpriteRenderer>(
		new SpriteRenderer(ellipse2, Util::loadTexture("Assets/Ellipse.png"), 1, 1, 0,
			{ }, 0, 0));

	ellipse2->setPosition(glm::vec3(8.5f, -2.0f, 1.0f));
	ellipse2->setScale({ 1.0f, 2.0f, 1.0f });
	ellipse2->setRotation(Z, 90.0f);
	ellipse2->update(FIXED_TIMESTEP);
	ellipse2->addComponent<Rigidbody2D>()->setRBType(STATIC);
	ellipse2->addComponent<CircleCollider2D>(new CircleCollider2D(ellipse2, { 0.5f, 0.5f, 0.0f }));
	*/

	/*
	// Irreg
	GameObject* irreg = new GameObject("Irregular Shape 1", &g_gameState.allGameObjects);
	g_gameState.allGameObjects.push_back(irreg);
	irreg->addComponent<SpriteRenderer>(
		new SpriteRenderer(irreg, Util::loadTexture("Assets/Irreg.png"), 1, 1, 0,
			{ },
			0, 0));
	irreg->setPosition(glm::vec3(3.5f, -2.0f, 1.0f));
	irreg->setScale({ 2.0f, 2.0f, 1.0f });
	irreg->update(FIXED_TIMESTEP);
	irreg->addComponent<Rigidbody2D>();
	irreg->addComponent<PolygonCollider2D>(
		new PolygonCollider2D(irreg,
			{   { 0.0f,  0.0f, 0.0f},
				{ 0.5f,  0.0f, 0.0f},
				{ 0.5f,  0.5f, 0.0f},
				{ 0.0f,  0.5f, 0.0f}, }));// ->setColliderScale({ 0.5f, 0.5f, 1.0f });
	irreg->addComponent<PolygonCollider2D>(
		new PolygonCollider2D(irreg,
			{   { 0.0f, -0.5f, 0.0f},
				{ 0.0f,  0.0f, 0.0f},
				{-0.5f,  0.0f, 0.0f}, }));// ->setColliderScale({ 0.5f, 0.5f, 1.0f });
	irreg->addComponent<PolygonCollider2D>(
		new PolygonCollider2D(irreg,
			{   {-0.5f,  0.0f, 0.0f},
				{ 0.0f,  0.0f, 0.0f},
				{ 0.0f,  0.5f, 0.0f}, }));// ->setColliderScale({ 0.5f, 0.5f, 1.0f });

	// Irreg
	GameObject* irreg2 = new GameObject("Irregular Shape 2", &g_gameState.allGameObjects);
	g_gameState.allGameObjects.push_back(irreg2);
	irreg2->addComponent<SpriteRenderer>(
		new SpriteRenderer(irreg2, Util::loadTexture("Assets/Irreg.png"), 1, 1, 0,
			{ },
			0, 0));
	irreg2->setPosition(glm::vec3(3.5f, 0.0f, 1.0f));
	irreg2->setScale({ 2.0f, 2.0f, 1.0f });
	irreg2->update(FIXED_TIMESTEP);
	irreg2->addComponent<Rigidbody2D>();// ->setRBType(STATIC);;
	irreg2->addComponent<PolygonCollider2D>(
		new PolygonCollider2D(irreg2,
			{   { 0.0f,  0.0f, 0.0f},
				{ 0.5f,  0.0f, 0.0f},
				{ 0.5f,  0.5f, 0.0f},
				{ 0.0f,  0.5f, 0.0f}, }));// ->setColliderScale({ 0.5f, 0.5f, 1.0f });
	irreg2->addComponent<PolygonCollider2D>(
		new PolygonCollider2D(irreg2,
			{   { 0.0f, -0.5f, 0.0f},
				{ 0.0f,  0.0f, 0.0f},
				{-0.5f,  0.0f, 0.0f}, }));// ->setColliderScale({ 0.5f, 0.5f, 1.0f });
	irreg2->addComponent<PolygonCollider2D>(
		new PolygonCollider2D(irreg2,
			{   {-0.5f,  0.0f, 0.0f},
				{ 0.0f,  0.0f, 0.0f},
				{ 0.0f,  0.5f, 0.0f}, }));// ->setColliderScale({ 0.5f, 0.5f, 1.0f });
	*/

	// Elevation Ground
	GameObject* tileMapEG1 = new GameObject("TileMapElevationGround", &g_gameState.allGameObjects);
	g_gameState.allGameObjects.push_back(tileMapEG1);
	tileMapEG1->addComponent<TileMap>(
		new TileMap(tileMapEG1, true, 29, 15,
		0, {}, 0,
		Util::loadLevelData("Assets/Maps/Level 1/Ground 1_1.png"),
		Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Terrain/Ground/Tilemap_Elevation.png"), 1.0f, 4, 8,
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
	tileMapEG1->setPosition(Z, 1.0f); //Use this to set z = elevation only


	// Player
	GameObject* player = new GameObject("Player", &g_gameState.allGameObjects);
	g_gameState.allGameObjects.push_back(player);
	player->addComponent<SpriteRenderer>(
		new SpriteRenderer(player, Util::loadTexture("Assets/Tiny Swords/Tiny Swords (Update 010)/Factions/Knights/Troops/Warrior/Blue/Warrior_Blue.png"), 6, 8, 0,
			{ { 0,  1,  2,  3,  4,  5},
				{ 6,  7,  8,  9, 10, 11},
				{12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
				{24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
				{36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
			},
			0, 12));
	player->addComponent<Rigidbody2D>();
	player->setPosition(glm::vec3(3.5f, 1.5f, 1.0f));
	player->update(FIXED_TIMESTEP);

	player->addComponent<PolygonCollider2D>(
			new PolygonCollider2D(player,
			{   {-0.5f,  0.5f, 0.0f},
				{ 0.5f,  0.5f, 0.0f},
				{ 0.5f, -0.5f, 0.0f},
				{-0.5f, -0.5f, 0.0f}, }))->setColliderScale({ 0.4f, 0.4f, 1.0f });
	//player->addComponent<CircleCollider2D>(new CircleCollider2D(player, { 1.0f, 1.0f, 1.0f }))->setColliderScale({ 0.25f, 0.25f, 0.25f });
	player->addComponent<PlayerMovementController>()->setMoveSpeed(2.0f);

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

	// Hold
	const Uint8* key_state = SDL_GetKeyboardState(NULL);

	// Process Objects
	for (auto gameObject : g_gameState.allGameObjects) {
		gameObject->processInput();
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
	for (auto gameObject : g_gameState.allGameObjects) {
		if (gameObject->getObjectID() == "Player") {
		//std::cout << gameObject->getObjectID() << " is at " << gameObject->getPosition().x << ", " << gameObject->getPosition().y << ", " << gameObject->getPosition().z << std::endl;

		//	gameObject->setRotation(Z, gameObject->getRotation().z + 1.0f);
		}
		if (gameObject->getObjectID() == "TileMapElevationGround") {
			//std::cout << gameObject->getObjectID() << " has " << gameObject->getComponentsSize() << std::endl;

			//	gameObject->setRotation(Z, gameObject->getRotation().z + 1.0f);
		}

		gameObject->update(FIXED_TIMESTEP);
		/*
		if (Entity* gameObjectEntity = gameObject->getComponent<Entity>()) {
			if (gameObjectEntity->getAlive()) {
				gameObjectEntity->update(FIXED_TIMESTEP);
			}
		}
		*/
	}

	// Move Camera to Player
	if (GameObject* player = g_gameState.findGameObject("Player")) {
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
}

void updateGameOver() {

}

void render() {
	glClear(GL_COLOR_BUFFER_BIT);

	// Sort allEntities to Render from top of map first (for illusion of depth)
	std::vector<GameObject*> sortedRender = { };
	for (auto gameObject : g_gameState.allGameObjects) {
		sortedRender.push_back(gameObject);
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
		//std::cout << "Rendering " << renderable->getObjectID() << std::endl;
		renderable->render(&g_program);
	}

	SDL_GL_SwapWindow(g_displayWindow);
}

void shutdown() {
	for (auto gameObject : g_gameState.allGameObjects)
	{
		delete gameObject;
	}
	g_gameState.allGameObjects.clear();
	SDL_Quit();
}
