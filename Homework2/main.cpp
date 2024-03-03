/**
* Author: John Li
* Assignment: Pong Clone
* Date due: 2024-03-02, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "stb_image.h"
#include "glm/mat4x4.hpp"                // 4x4 Matrix
#include "glm/gtc/matrix_transform.hpp"  // Matrix transformation methods
#include "ShaderProgram.h"               
#include <map>
#include <string>
#include <vector>

// Window
const int WINDOW_WIDTH = 640 * 2;
const int WINDOW_HEIGHT = 480 * 2;

// BG
float g_bgR = 0.3f;
float g_bgG = 0.6f;
float g_bgB = 0.9f;
float g_bgA = 1.0f;

// Camera
const int VIEWPORT_X = 0;
const int VIEWPORT_Y = 0;
const int VIEWPORT_WIDTH = WINDOW_WIDTH;
const int VIEWPORT_HEIGHT = WINDOW_HEIGHT;

// Shader filepaths
const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl";
const char F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

// Objects (models) Globals
struct ModelObject {
	glm::mat4 modelMatrix;
	GLuint modelTexture;
	bool atlas;
	int atlasRows;
	int atlasColumns;
	int atlasIndex;
	int* atlasIndices;
	int** atlasAnims;
	glm::vec3 position;
	glm::vec3 movement;
	float moveSpeed;
	std::map<std::string, ModelObject> collidingWith;
};
std::map<std::string, ModelObject> g_modelObjects;

// Display Globals
SDL_Window* g_displayWindow;

// Game Globals
bool g_gameIsRunning = true;
bool g_WaitToQuit = false;

float g_seconds;
float g_previousSeconds;
float g_deltaTime;
float g_previousTicks;

const int FRAMES_PER_SECOND = 4;
float g_animationTime;

bool g_singleplayer = false;
float PADDLE_1_ORIGIN_X = -3.5f;
float PADDLE_2_ORIGIN_X = 3.5f;
float g_theta;

// Shader Globals
ShaderProgram g_program;
glm::mat4 g_viewMatrix;        // Defines camera location and orientation
glm::mat4 g_projectionMatrix;  // Defines camera clip panes, field of view, projection method, etc.

// Image Globals
const int NUMBER_OF_TEXTURES = 1; // Number of textures generated per model
const GLint LEVEL_OF_DETAIL = 0; // base image level
const GLint TEXTURE_BORDER = 0;

void initialize();
GLuint loadTexture(const char* filepath);
void processInput();
void update();
void updateDeltaTime();
void render();
void drawObject(ModelObject modelObject);
void drawFromAtlas(GLuint texture_id, int index, int rows, int cols);
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
	g_displayWindow = SDL_CreateWindow("Hello, Delta Time!",
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

	// Initialize Objects (models and assets)
	
	// Player 1 / Paddle 1
	g_modelObjects["Paddle1"] = { glm::mat4(1.0f), loadTexture("Assets/MilitiaAtlas.png"), true, 2, 4, 0,
		new int[4] { 0, 1, 2, 3 },
		new int* [2] {
			new int[4] { 0, 1, 2, 3 }, 
			new int[4] { 4, 5, 6, 7 },
		},
		glm::vec3(PADDLE_1_ORIGIN_X, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		5.0f,
	};
	g_modelObjects["Paddle1"].modelMatrix = glm::mat4(1.0f);
	g_modelObjects["Paddle1"].modelMatrix = glm::translate(g_modelObjects["Paddle1"].modelMatrix, glm::vec3(PADDLE_1_ORIGIN_X, 0.0f, 0.0f));

	// Player 2 / Paddle 2
	g_modelObjects["Paddle2"] = { glm::mat4(1.0f), loadTexture("Assets/GoblinWarriorAtlas.png"), true, 2, 4, 0,
		new int[4] { 0, 1, 2, 3 },
		new int* [2] {
			new int[4] { 0, 1, 2, 3 },
			new int[4] { 4, 5, 6, 7 },
		},
		glm::vec3(PADDLE_2_ORIGIN_X, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		5.0f,
	};
	g_modelObjects["Paddle2"].modelMatrix = glm::mat4(1.0f);
	g_modelObjects["Paddle2"].modelMatrix = glm::rotate(g_modelObjects["Paddle2"].modelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	g_modelObjects["Paddle2"].modelMatrix = glm::translate(g_modelObjects["Paddle2"].modelMatrix, glm::vec3(PADDLE_2_ORIGIN_X, 0.0f, 0.0f));

	// Ball1
	g_modelObjects["Ball1"] = { glm::mat4(1.0f), loadTexture("Assets/ArcaneBall.png"), false, 1, 1, 0,
		nullptr,
		nullptr,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(((std::rand() % 2) == 0 ? -1 : 1) , (((float)(std::rand() % 30) + 30.0f) / 100.0f), 0.0f),
		2.0f,
	};

	/*
	// Ball2
	g_modelObjects["Ball2"] = { glm::mat4(1.0f), loadTexture("Assets/ArcaneBall.png"), false, 1, 1, 0,
		nullptr,
		nullptr,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(((std::rand() % 2) == 0 ? -1 : 1) , (((float)(std::rand() % 30) + 30.0f) / 100.0f), 0.0f),
		2.0f,
	};

	// Ball3
	g_modelObjects["Ball3"] = { glm::mat4(1.0f), loadTexture("Assets/ArcaneBall.png"), false, 1, 1, 0,
		nullptr,
		nullptr,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(((std::rand() % 2) == 0 ? -1 : 1) , (((float)(std::rand() % 30) + 30.0f) / 100.0f), 0.0f),
		2.0f,
	};
	*/

	// Win Screen
	g_modelObjects["WinScreen"] = { glm::mat4(1.0f), loadTexture("Assets/WinScreenAtlas.png"), true, 1, 3, 0,
		new int[1] { 0 },
		new int* [3] {
			new int[1] { 0 },
			new int[1] { 1 },
			new int[1] { 2 },
		},
		glm::vec3(0.0f, 4.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		0.0f,
	};
	g_modelObjects["WinScreen"].modelMatrix = glm::mat4(1.0f);
	g_modelObjects["WinScreen"].modelMatrix = glm::translate(g_modelObjects["WinScreen"].modelMatrix, g_modelObjects["WinScreen"].position);


	// Walls
	char wallName[7];

	// Up Left
	g_modelObjects["WallUL"] = { glm::mat4(1.0f), loadTexture("Assets/WallAtlas.png"), true, 4, 4, 0,
		new int[1] { 0 },
		new int* [1] {
			new int[1] { 0 },
		},
		glm::vec3(-4.5f, 3.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		0.0f,
	};
	g_modelObjects["WallUL"].modelMatrix = glm::mat4(1.0f);
	g_modelObjects["WallUL"].modelMatrix = glm::translate(g_modelObjects["WallUL"].modelMatrix, g_modelObjects["WallUL"].position);

	// Up
	for (int i = 0; i < 8; i++) {
		snprintf(wallName, 7, "WallU%d", i);

		g_modelObjects[wallName] = { glm::mat4(1.0f), loadTexture("Assets/WallAtlas.png"), true, 4, 4, 0,
			new int[1] { 13 },
			new int* [1] {
				new int[1] { 13 },
			},
			glm::vec3(-3.5f+i, 3.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			0.0f,
		};
		g_modelObjects[wallName].modelMatrix = glm::mat4(1.0f);
		g_modelObjects[wallName].modelMatrix = glm::translate(g_modelObjects[wallName].modelMatrix, g_modelObjects[wallName].position);
		std::cout << wallName << g_modelObjects[wallName].position.x << std::endl;

	}

	// Up Right
	g_modelObjects["WallUR"] = { glm::mat4(1.0f), loadTexture("Assets/WallAtlas.png"), true, 4, 4, 0,
		new int[1] { 2 },
		new int* [1] {
			new int[1] { 2 },
		},
		glm::vec3(4.5f, 3.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		0.0f,
	};
	g_modelObjects["WallUR"].modelMatrix = glm::mat4(1.0f);
	g_modelObjects["WallUR"].modelMatrix = glm::translate(g_modelObjects["WallUR"].modelMatrix, g_modelObjects["WallUR"].position);

	// Right
	for (int i = 0; i < 5; i++) {
		snprintf(wallName, 7, "WallR%d", i);

		g_modelObjects[wallName] = { glm::mat4(1.0f), loadTexture("Assets/WallAtlas.png"), true, 4, 4, 0,
			new int[1] { 7 },
			new int* [1] {
				new int[1] { 7 },
			},
			glm::vec3(-4.5f, 2.0f - i, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			0.0f,
		};
		g_modelObjects[wallName].modelMatrix = glm::mat4(1.0f);
		g_modelObjects[wallName].modelMatrix = glm::translate(g_modelObjects[wallName].modelMatrix, g_modelObjects[wallName].position);
		std::cout << wallName << g_modelObjects[wallName].position.y << std::endl;
	}

	// Left
	for (int i = 0; i < 5; i++) {
		snprintf(wallName, 7, "WallL%d", i);

		g_modelObjects[wallName] = { glm::mat4(1.0f), loadTexture("Assets/WallAtlas.png"), true, 4, 4, 0,
			new int[1] { 7 },
			new int* [1] {
				new int[1] { 7 },
			},
			glm::vec3(4.5f, 2.0f - i, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			0.0f,
		};
		g_modelObjects[wallName].modelMatrix = glm::mat4(1.0f);
		g_modelObjects[wallName].modelMatrix = glm::translate(g_modelObjects[wallName].modelMatrix, g_modelObjects[wallName].position);
		std::cout << wallName << g_modelObjects[wallName].position.y << std::endl;
	}

	// Down Left
	g_modelObjects["WallDL"] = { glm::mat4(1.0f), loadTexture("Assets/WallAtlas.png"), true, 4, 4, 0,
		new int[1] { 8 },
		new int* [1] {
			new int[1] { 8 },
		},
		glm::vec3(-4.5f, -3.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		0.0f,
	};
	g_modelObjects["WallDL"].modelMatrix = glm::mat4(1.0f);
	g_modelObjects["WallDL"].modelMatrix = glm::translate(g_modelObjects["WallDL"].modelMatrix, g_modelObjects["WallDL"].position);

	// Down Right
	g_modelObjects["WallDR"] = { glm::mat4(1.0f), loadTexture("Assets/WallAtlas.png"), true, 4, 4, 0,
		new int[1] { 10 },
		new int* [1] {
			new int[1] { 10 },
		},
		glm::vec3(4.5f, -3.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		0.0f,
	};
	g_modelObjects["WallDR"].modelMatrix = glm::mat4(1.0f);
	g_modelObjects["WallDR"].modelMatrix = glm::translate(g_modelObjects["WallDR"].modelMatrix, g_modelObjects["WallDR"].position);

	// Down
	for (int i = 0; i < 8; i++) {
		snprintf(wallName, 7, "WallD%d", i);

		g_modelObjects[wallName] = { glm::mat4(1.0f), loadTexture("Assets/WallAtlas.png"), true, 4, 4, 0,
			new int[1] { 13 },
			new int* [1] {
				new int[1] { 13 },
			},
			glm::vec3(-3.5f + i, -3.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			0.0f,
		};
		g_modelObjects[wallName].modelMatrix = glm::mat4(1.0f);
		g_modelObjects[wallName].modelMatrix = glm::translate(g_modelObjects[wallName].modelMatrix, g_modelObjects[wallName].position);
		std::cout << wallName << g_modelObjects[wallName].position.x << std::endl;

	}
}

GLuint loadTexture(const char* filepath) {
	// Loading the image file
	int width, height, number_of_components;
	unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);
	if (image == NULL) {
		std::cout << "Unable to load image at: " << filepath << std::endl;
		assert(false);
	}

	// Generate and bind a texture ID to image
	GLuint texture_id;
	glGenTextures(NUMBER_OF_TEXTURES, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);

	// Set texture filter parameters, NEAREST for pixel art
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Release file from memory to avoid memory leak
	stbi_image_free(image);

	return texture_id;
}

void processInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			g_gameIsRunning = false;
			break;
		case SDL_KEYDOWN: // Toggle
			switch (event.key.keysym.sym) {
				case SDLK_t:
					g_singleplayer = !g_singleplayer;
					g_modelObjects["Paddle2"].movement.y = 1.0f;
					break;
			}
		}
	}

	if (g_WaitToQuit) {
		return;
	}

	const Uint8* key_state = SDL_GetKeyboardState(NULL);

	// P1 Vertical Tracking
	if ((key_state[SDL_SCANCODE_W]) && (key_state[SDL_SCANCODE_S]))
	{
		g_modelObjects["Paddle1"].movement.y = 0.0f;
		g_modelObjects["Paddle1"].atlasIndices = g_modelObjects["Paddle1"].atlasAnims[0];
	}
	else if (key_state[SDL_SCANCODE_W])
	{
		bool free = true;
		float xColDistance;
		float yColDistance;

		for (const std::pair<std::string, ModelObject>& modelObjectPair : g_modelObjects["Paddle1"].collidingWith) {
			xColDistance = fabs(g_modelObjects["Paddle1"].position.x - modelObjectPair.second.position.x) - ((1 + 1) / 2.0f);
			yColDistance = fabs(g_modelObjects["Paddle1"].position.y - modelObjectPair.second.position.y) - ((1 + 1) / 2.0f);

			if ((modelObjectPair.second.position.y > g_modelObjects["Paddle1"].position.y) &&
				(xColDistance < yColDistance)) {
				//std::cout << ("Can't Move Up! Blocked by") << modelObjectPair.first << std::endl;
				free = false;

				g_modelObjects["Paddle1"].movement.y = 0.0f;
				g_modelObjects["Paddle1"].atlasIndices = g_modelObjects["Paddle1"].atlasAnims[0];
			}
		}

		if (free) {
			g_modelObjects["Paddle1"].movement.y = 1.0f;
			g_modelObjects["Paddle1"].atlasIndices = g_modelObjects["Paddle1"].atlasAnims[1];
		}
	}
	else if (key_state[SDL_SCANCODE_S])
	{
		bool free = true;
		float xColDistance;
		float yColDistance;

		for (const std::pair<std::string, ModelObject>& modelObjectPair : g_modelObjects["Paddle1"].collidingWith) {
			xColDistance = fabs(g_modelObjects["Paddle1"].position.x - modelObjectPair.second.position.x) - ((1 + 1) / 2.0f);
			yColDistance = fabs(g_modelObjects["Paddle1"].position.y - modelObjectPair.second.position.y) - ((1 + 1) / 2.0f);

			if ((modelObjectPair.second.position.y < g_modelObjects["Paddle1"].position.y) &&
				(xColDistance < yColDistance)) {
				//std::cout << ("Can't Move Down! Blocked by") << modelObjectPair.first << std::endl;
				free = false;

				g_modelObjects["Paddle1"].movement.y = 0.0f;
				g_modelObjects["Paddle1"].atlasIndices = g_modelObjects["Paddle1"].atlasAnims[0];
			}
		}

		if (free) {
			g_modelObjects["Paddle1"].movement.y = -1.0f;
			g_modelObjects["Paddle1"].atlasIndices = g_modelObjects["Paddle1"].atlasAnims[1];
		}
	}
	else
	{
		g_modelObjects["Paddle1"].movement.y = 0.0f;
		g_modelObjects["Paddle1"].atlasIndices = g_modelObjects["Paddle1"].atlasAnims[0];
	}

	// P1 Normalization
	if (glm::length(g_modelObjects["Paddle1"].movement) > 1.0f)
	{
		g_modelObjects["Paddle1"].movement = glm::normalize(g_modelObjects["Paddle1"].movement);
	}

	// P2 Vertical Tracking
	if (g_singleplayer) {
		return;
	}

	if ((key_state[SDL_SCANCODE_UP]) && (key_state[SDL_SCANCODE_DOWN]))
	{
		g_modelObjects["Paddle2"].movement.y = 0.0f;
		g_modelObjects["Paddle2"].atlasIndices = g_modelObjects["Paddle2"].atlasAnims[0];
	}
	else if (key_state[SDL_SCANCODE_UP])
	{
		bool free = true;
		float xColDistance;
		float yColDistance;

		for (const std::pair<std::string, ModelObject>& modelObjectPair : g_modelObjects["Paddle2"].collidingWith) {
			xColDistance = fabs(g_modelObjects["Paddle2"].position.x - modelObjectPair.second.position.x) - ((1 + 1) / 2.0f);
			yColDistance = fabs(g_modelObjects["Paddle2"].position.y - modelObjectPair.second.position.y) - ((1 + 1) / 2.0f);

			if ((modelObjectPair.second.position.y > g_modelObjects["Paddle2"].position.y) &&
				(xColDistance < yColDistance)) {
				//std::cout << ("Can't Move Up! Blocked by") << modelObjectPair.first << std::endl;
				free = false;

				g_modelObjects["Paddle2"].movement.y = 0.0f;
				g_modelObjects["Paddle2"].atlasIndices = g_modelObjects["Paddle2"].atlasAnims[0];
			}
		}

		if (free) {
			g_modelObjects["Paddle2"].movement.y = 1.0f;
			g_modelObjects["Paddle2"].atlasIndices = g_modelObjects["Paddle2"].atlasAnims[1];
		}
	}
	else if (key_state[SDL_SCANCODE_DOWN])
	{
		bool free = true;
		float xColDistance;
		float yColDistance;

		for (const std::pair<std::string, ModelObject>& modelObjectPair : g_modelObjects["Paddle2"].collidingWith) {
			xColDistance = fabs(g_modelObjects["Paddle2"].position.x - modelObjectPair.second.position.x) - ((1 + 1) / 2.0f);
			yColDistance = fabs(g_modelObjects["Paddle2"].position.y - modelObjectPair.second.position.y) - ((1 + 1) / 2.0f);

			if ((modelObjectPair.second.position.y < g_modelObjects["Paddle2"].position.y) &&
				(xColDistance < yColDistance)) {
				//std::cout << ("Can't Move Up! Blocked by") << modelObjectPair.first << std::endl;
				free = false;

				g_modelObjects["Paddle2"].movement.y = 0.0f;
				g_modelObjects["Paddle2"].atlasIndices = g_modelObjects["Paddle2"].atlasAnims[0];
			}
		}

		if (free) {
			g_modelObjects["Paddle2"].movement.y = -1.0f;
			g_modelObjects["Paddle2"].atlasIndices = g_modelObjects["Paddle2"].atlasAnims[1];
		}
	}
	else
	{
		g_modelObjects["Paddle2"].movement.y = 0.0f;
		g_modelObjects["Paddle2"].atlasIndices = g_modelObjects["Paddle2"].atlasAnims[0];
	}

	// P2 Normalization
	if (glm::length(g_modelObjects["Paddle2"].movement) > 1.0f)
	{
		g_modelObjects["Paddle2"].movement = glm::normalize(g_modelObjects["Paddle2"].movement);
	}
}

void update() {
	updateDeltaTime();
	//std::cout << (int)g_seconds << std::endl;

	// WinScreen
	g_modelObjects["WinScreen"].modelMatrix = glm::mat4(1.0f);
	g_modelObjects["WinScreen"].modelMatrix = glm::translate(g_modelObjects["WinScreen"].modelMatrix, g_modelObjects["WinScreen"].position);


	float xColDistance;
	float yColDistance;

	// Check all collisions against p1
	for (const std::pair<std::string, ModelObject>& modelObjectPair : g_modelObjects) {
		if (modelObjectPair.first != "Paddle1") {
			xColDistance = fabs(g_modelObjects["Paddle1"].position.x - modelObjectPair.second.position.x) - ((1 + 1) / 2.0f);
			yColDistance = fabs(g_modelObjects["Paddle1"].position.y - modelObjectPair.second.position.y) - ((1 + 1) / 2.0f);

			if ((xColDistance < -0.75f) && (yColDistance < -0.5f) &&
				(g_modelObjects["Paddle1"].collidingWith.count(modelObjectPair.first) == 0)) {
				//std::cout << ("P1 NEW COLLISION!") << modelObjectPair.first << std::endl;
				g_modelObjects["Paddle1"].collidingWith[modelObjectPair.first] = modelObjectPair.second;
			}
			else if (((xColDistance >= -0.75f) || (yColDistance >= -0.5f)) &&
				(g_modelObjects["Paddle1"].collidingWith.count(modelObjectPair.first) == 1)) {
				g_modelObjects["Paddle1"].collidingWith.erase(modelObjectPair.first);
				//std::cout << ("P1 ended collision with ") << modelObjectPair.first << std::endl;
			}
		}
	}

	// Check all collisions against p2
	for (const std::pair<std::string, ModelObject>& modelObjectPair : g_modelObjects) {
		if (modelObjectPair.first != "Paddle2") {
			xColDistance = fabs(g_modelObjects["Paddle2"].position.x - modelObjectPair.second.position.x) - ((1 + 1) / 2.0f);
			yColDistance = fabs(g_modelObjects["Paddle2"].position.y - modelObjectPair.second.position.y) - ((1 + 1) / 2.0f);

			if ((xColDistance < -0.75f) && (yColDistance < -0.5f) &&
				(g_modelObjects["Paddle2"].collidingWith.count(modelObjectPair.first) == 0)) {
				//std::cout << ("P2 NEW COLLISION!") << modelObjectPair.first << std::endl;
				g_modelObjects["Paddle2"].collidingWith[modelObjectPair.first] = modelObjectPair.second;
			}
			else if (((xColDistance >= -0.75f) || (yColDistance >= -0.5f)) &&
				(g_modelObjects["Paddle2"].collidingWith.count(modelObjectPair.first) == 1)) {
				g_modelObjects["Paddle2"].collidingWith.erase(modelObjectPair.first);
				//std::cout << ("P2 ended collision with ") << modelObjectPair.first << std::endl;
			}
		}
	}

	// Check all collisions against ball
	for (const std::pair<std::string, ModelObject>& modelObjectPair : g_modelObjects) {
		if (modelObjectPair.first != "Ball1") {
			xColDistance = fabs(g_modelObjects["Ball1"].position.x - modelObjectPair.second.position.x) - ((1 + 1) / 2.0f);
			yColDistance = fabs(g_modelObjects["Ball1"].position.y - modelObjectPair.second.position.y) - ((1 + 1) / 2.0f);

			if ((xColDistance < -0.5f) && (yColDistance < -0.5f) &&
				(g_modelObjects["Ball1"].collidingWith.count(modelObjectPair.first) == 0)) {
				std::cout << ("Ball NEW COLLISION!") << modelObjectPair.first << std::endl;
				g_modelObjects["Ball1"].collidingWith[modelObjectPair.first] = modelObjectPair.second;

				if (modelObjectPair.first.find("WallR") != std::string::npos) {
					std::cout << "win1" << std::endl;
					g_modelObjects["WinScreen"].atlasIndices = g_modelObjects["WinScreen"].atlasAnims[2];
					g_modelObjects["WinScreen"].modelMatrix = glm::mat4(1.0f);
					g_modelObjects["WinScreen"].position.y = 0.0f;
					g_modelObjects["WinScreen"].modelMatrix = glm::translate(g_modelObjects["WinScreen"].modelMatrix, g_modelObjects["WinScreen"].position);
					g_WaitToQuit = true;
				}
				else if (modelObjectPair.first.find("WallL") != std::string::npos) {
					std::cout << "win2" << std::endl;
					g_modelObjects["WinScreen"].atlasIndices = g_modelObjects["WinScreen"].atlasAnims[1];
					g_modelObjects["WinScreen"].modelMatrix = glm::mat4(1.0f);
					g_modelObjects["WinScreen"].position.y = 0.0f;
					g_modelObjects["WinScreen"].modelMatrix = glm::translate(g_modelObjects["WinScreen"].modelMatrix, g_modelObjects["WinScreen"].position);
					g_WaitToQuit = true;

				}

			}
			else if (((xColDistance >= -0.5f) || (yColDistance >= -0.5f)) &&
				(g_modelObjects["Ball1"].collidingWith.count(modelObjectPair.first) == 1)) {
				g_modelObjects["Ball1"].collidingWith.erase(modelObjectPair.first);
				std::cout << ("Ball ended collision with ") << modelObjectPair.first << std::endl;
			}
		}
	}

	// Anim stuff
	g_animationTime += g_deltaTime;
	float seconds_per_frame = (float)1 / FRAMES_PER_SECOND;

	if (g_animationTime >= seconds_per_frame)
	{
		g_animationTime = 0.0f;

		// P1 Animation
		g_modelObjects["Paddle1"].atlasIndex++;
		if (g_modelObjects["Paddle1"].atlasIndex >= g_modelObjects["Paddle1"].atlasColumns)
		{
			g_modelObjects["Paddle1"].atlasIndex = 0;
		}

		// P2 Animation
		g_modelObjects["Paddle2"].atlasIndex++;
		if (g_modelObjects["Paddle2"].atlasIndex >= g_modelObjects["Paddle2"].atlasColumns)
		{
			g_modelObjects["Paddle2"].atlasIndex = 0;
		}
	}

	// 2 seconds to load, game should only move after players are able to see
	if (g_seconds <= 2.0f) {
		return;
	}

	// P1 Movement
	g_modelObjects["Paddle1"].modelMatrix = glm::mat4(1.0f);
	g_modelObjects["Paddle1"].position += g_modelObjects["Paddle1"].movement * g_modelObjects["Paddle1"].moveSpeed * g_deltaTime;
	g_modelObjects["Paddle1"].modelMatrix = glm::translate(g_modelObjects["Paddle1"].modelMatrix, g_modelObjects["Paddle1"].position);
	//std::cout << g_modelObjects["Paddle1"].atlasIndices[0] << std::endl;


	// P2 Movement
	g_modelObjects["Paddle2"].modelMatrix = glm::mat4(1.0f);
	if (g_singleplayer == false) {
		g_modelObjects["Paddle2"].position += g_modelObjects["Paddle2"].movement * g_modelObjects["Paddle2"].moveSpeed * g_deltaTime;
		g_modelObjects["Paddle2"].modelMatrix = glm::translate(g_modelObjects["Paddle2"].modelMatrix, g_modelObjects["Paddle2"].position);
	}
	else {

		for (const std::pair<std::string, ModelObject>& modelObjectPair : g_modelObjects["Paddle2"].collidingWith) {
			xColDistance = fabs(g_modelObjects["Paddle2"].position.x - modelObjectPair.second.position.x) - ((1 + 1) / 2.0f);
			yColDistance = fabs(g_modelObjects["Paddle2"].position.y - modelObjectPair.second.position.y) - ((1 + 1) / 2.0f);

			if ((modelObjectPair.second.position.y > g_modelObjects["Paddle2"].position.y) &&
				(xColDistance < yColDistance)) {
				//std::cout << ("Can't Move Up! Blocked by") << modelObjectPair.first << std::endl;

				g_modelObjects["Paddle2"].movement.y = -1.0f;
				g_modelObjects["Paddle2"].atlasIndices = g_modelObjects["Paddle2"].atlasAnims[1];
			}
		}

		for (const std::pair<std::string, ModelObject>& modelObjectPair : g_modelObjects["Paddle2"].collidingWith) {
			xColDistance = fabs(g_modelObjects["Paddle2"].position.x - modelObjectPair.second.position.x) - ((1 + 1) / 2.0f);
			yColDistance = fabs(g_modelObjects["Paddle2"].position.y - modelObjectPair.second.position.y) - ((1 + 1) / 2.0f);

			if ((modelObjectPair.second.position.y < g_modelObjects["Paddle2"].position.y) &&
				(xColDistance < yColDistance)) {
				//std::cout << ("Can't Move Down! Blocked by") << modelObjectPair.first << std::endl;

				g_modelObjects["Paddle2"].movement.y = 1.0f;
				g_modelObjects["Paddle2"].atlasIndices = g_modelObjects["Paddle2"].atlasAnims[1];
			}
		}

		g_modelObjects["Paddle2"].position += g_modelObjects["Paddle2"].movement * g_modelObjects["Paddle2"].moveSpeed * g_deltaTime;
		g_modelObjects["Paddle2"].modelMatrix = glm::translate(g_modelObjects["Paddle2"].modelMatrix, g_modelObjects["Paddle2"].position);
	}
	g_modelObjects["Paddle2"].modelMatrix = glm::rotate(g_modelObjects["Paddle2"].modelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Ball Movement
	for (const std::pair<std::string, ModelObject>& modelObjectPair : g_modelObjects["Ball1"].collidingWith) {
		xColDistance = fabs(g_modelObjects["Ball1"].position.x - modelObjectPair.second.position.x) - ((1 + 1) / 2.0f);
		yColDistance = fabs(g_modelObjects["Ball1"].position.y - modelObjectPair.second.position.y) - ((1 + 1) / 2.0f);

		if ((modelObjectPair.second.position.y > g_modelObjects["Ball1"].position.y) &&
			((xColDistance <= yColDistance) || (yColDistance < -0.85f))) {
			std::cout << ("Can't Move Up! Blocked by") << modelObjectPair.first << std::endl;

			if (g_modelObjects["Ball1"].movement.y > 0.0f) {
				g_modelObjects["Ball1"].movement.y = -fabs(g_modelObjects["Ball1"].movement.y);
			}
		}
		else if ((modelObjectPair.second.position.y < g_modelObjects["Ball1"].position.y) &&
			((xColDistance <= yColDistance) || (yColDistance < -0.85f))) {
			std::cout << ("Can't Move Down! Blocked by") << modelObjectPair.first << std::endl;

			if (g_modelObjects["Ball1"].movement.y < 0.0f) {
				g_modelObjects["Ball1"].movement.y = fabs(g_modelObjects["Ball1"].movement.y);
			}
		}
		if ((modelObjectPair.second.position.x > g_modelObjects["Ball1"].position.x) &&
			((xColDistance >= yColDistance) || (xColDistance < -0.85f))) {
			std::cout << ("Can't Move Right! Blocked by") << modelObjectPair.first << std::endl;

			if (g_modelObjects["Ball1"].movement.x > 0.0f) {
				g_modelObjects["Ball1"].movement.x = -fabs(g_modelObjects["Ball1"].movement.x);
			}
		}
		else if ((modelObjectPair.second.position.x < g_modelObjects["Ball1"].position.x) &&
			((xColDistance >= yColDistance) || (xColDistance < -0.85f))) {
			std::cout << ("Can't Move Left! Blocked by") << modelObjectPair.first << std::endl;

			if (g_modelObjects["Ball1"].movement.x < 0.0f) {
				g_modelObjects["Ball1"].movement.x = fabs(g_modelObjects["Ball1"].movement.x);
			}
		}
	}

	g_modelObjects["Ball1"].modelMatrix = glm::mat4(1.0f);
	g_modelObjects["Ball1"].position += g_modelObjects["Ball1"].movement * g_modelObjects["Ball1"].moveSpeed * g_deltaTime;
	g_modelObjects["Ball1"].modelMatrix = glm::translate(g_modelObjects["Ball1"].modelMatrix, g_modelObjects["Ball1"].position);

	g_modelObjects["Ball1"].moveSpeed += (g_seconds / 50000.0f);
	//std::cout << ("Ball Speed: ") << g_modelObjects["Ball1"].moveSpeed << std::endl;
}

void updateDeltaTime() {
	float ticks = (float)SDL_GetTicks() / 1000;  // get the current number of ticks by millisecond per second
	g_deltaTime = ticks - g_previousTicks;     // delta time = change in time since last frame
	g_previousTicks = ticks;
	g_seconds += 1.0f * g_deltaTime;    // get the current number of seconds that have passed
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT);

	// Vertices
	float vertices[] = {
		-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,  // triangle 1
		-0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f   // triangle 2
	};
	glVertexAttribPointer(g_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(g_program.get_position_attribute());

	// Textures
	float texture_coordinates[] = {
		0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,     // triangle 1
		0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,     // triangle 2
	};
	glVertexAttribPointer(g_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates);
	glEnableVertexAttribArray(g_program.get_tex_coordinate_attribute());

	// Bind all Textures
	for (const std::pair<std::string, ModelObject>& modelObjectPair : g_modelObjects) {
		drawObject(modelObjectPair.second);
	}

	glDisableVertexAttribArray(g_program.get_position_attribute());
	glDisableVertexAttribArray(g_program.get_tex_coordinate_attribute());

	SDL_GL_SwapWindow(g_displayWindow);
}

void drawObject(ModelObject modelObject) {
	g_program.set_model_matrix(modelObject.modelMatrix);
	
	//std::cout << modelObject.atlasIndices[0] << std::endl;

	if (modelObject.atlas) {
		drawFromAtlas(modelObject.modelTexture, modelObject.atlasIndices[modelObject.atlasIndex], modelObject.atlasRows, modelObject.atlasColumns);
	}
	else {
		g_program.set_model_matrix(modelObject.modelMatrix);
		glBindTexture(GL_TEXTURE_2D, modelObject.modelTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

void drawFromAtlas(GLuint texture_id, int index, int rows, int cols)
{
	// Step 1: Calculate the UV location of the indexed frame
	float u_coord = (float)(index % cols) / (float)cols;
	float v_coord = (float)(index / cols) / (float)rows;

	// Step 2: Calculate its UV size
	float width = 1.0f / (float)cols;
	float height = 1.0f / (float)rows;

	// Step 3: Just as we have done before, match the texture coordinates to the vertices
	float tex_coords[] =
	{
		u_coord, v_coord + height, u_coord + width, v_coord + height, u_coord + width, v_coord,
		u_coord, v_coord + height, u_coord + width, v_coord, u_coord, v_coord
	};

	float vertices[] =
	{
		-0.5, -0.5, 0.5, -0.5,  0.5, 0.5,
		-0.5, -0.5, 0.5,  0.5, -0.5, 0.5
	};

	// Step 4: And render
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glVertexAttribPointer(g_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(g_program.get_position_attribute());

	glVertexAttribPointer(g_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
	glEnableVertexAttribArray(g_program.get_tex_coordinate_attribute());

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(g_program.get_position_attribute());
	glDisableVertexAttribArray(g_program.get_tex_coordinate_attribute());
}

void shutdown() {
	SDL_Quit();
}
	