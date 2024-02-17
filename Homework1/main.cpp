/**
* Author: John Li
* Assignment: Simple 2D Scene
* Date due: 2024-02-17, 11:59pm
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
};
std::map<std::string, ModelObject> g_modelObjects;

// Display Globals
SDL_Window * g_displayWindow;

// Game Globals
bool g_gameIsRunning = true;
float g_seconds;
float g_previousSeconds;
float g_deltaTime;
float g_previousTicks;
bool g_arrowRight = true;
float g_arrowX = 0.0f;
float g_arrowY = 0.0f;
float g_arrowRotate = 0.0f;
float g_arrowScale = 1.0f;
float g_archerX = 0.0f;
float g_archerY = 0.0f;
float g_archerRotate = 0.0f;


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
	g_modelObjects["Archer"] = { glm::mat4(1.0f), loadTexture("Assets/Archers_Idle.png") };
	g_modelObjects["Arrow"] = { glm::mat4(1.0f), loadTexture("Assets/Archers_Projectile.png") };
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

	// Release file from memory to avoid memory leak
	stbi_image_free(image);

	return texture_id;
}

void processInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
		{
			g_gameIsRunning = false;
		}
	}
}

void update() {
	updateDeltaTime();
	std::cout << (int)g_seconds << std::endl;

	// Change bg color every 2 seconds
	if ((int)g_seconds - g_previousSeconds >= 2) {
		g_previousSeconds = (int)g_seconds;
		float tempColor = g_bgR;
		g_bgR = g_bgG;
		g_bgG = g_bgB;
		g_bgB = tempColor;
		glClearColor(g_bgR, g_bgG, g_bgB, g_bgA);
	}

	// Arrow Movement Calculations
	if ((g_arrowX < 4) && g_arrowRight) {
		g_arrowX += (4.0f * g_deltaTime) * (4.1f - std::abs(g_arrowX));
		g_arrowY -= (4.0f * g_deltaTime) * (1.1f - std::abs(g_arrowY));
		if (g_arrowScale <= 3.0f) {
			g_arrowScale += (1.1f * g_deltaTime) * (4.1f - std::abs(g_arrowX));
		}
	}
	else if ((g_arrowRotate < 180) && g_arrowRight) {
		g_arrowRotate += 360.0f * g_deltaTime;
		if (g_arrowScale > 1.0f) {
			g_arrowScale -= (0.1f * g_deltaTime) * (abs(180.0f - g_arrowRotate));
		}
	}
	else if (g_arrowRight) {
		g_arrowRight = false;
	}
	else if ((g_arrowX > -4) && !g_arrowRight) {
		g_arrowX -= (4.0f * g_deltaTime) * (4.1f - std::abs(g_arrowX));
		g_arrowY += (4.0f * g_deltaTime) * (1.1f - std::abs(g_arrowY));

		if (g_arrowScale <= 3.0f) {
			g_arrowScale += (1.1f * g_deltaTime) * (4.1f - std::abs(g_arrowX));
		}
	}
	else if ((g_arrowRotate > 0) && !g_arrowRight) {
		g_arrowRotate -= 360.0f * g_deltaTime;
		if (g_arrowScale > 1.0f) {
			g_arrowScale -= (0.1f * g_deltaTime) * (abs(0.0f - g_arrowRotate));
		}
	}
	else if (!g_arrowRight) {
		g_arrowRight = true;
	}

	// Archer Movement Calculations
	if ((g_arrowX >= 0) && (g_archerY <= 2)) {
		g_archerY += (3.0f * g_deltaTime) * (2.1f - std::abs(g_archerY));
		if ((g_archerX < 1) && g_arrowRight) {
			g_archerX += (2.0f * g_deltaTime);
		}
	}
	else if ((g_arrowX < 0) && (g_archerY >= -2)) {
		g_archerY -= (3.0f * g_deltaTime) * (2.1f - std::abs(g_archerY));
		if ((g_archerX > -1) && !g_arrowRight) {
			g_archerX -= (2.0f * g_deltaTime);
		}
	}

	// Arrow transformations, with movement in respect to Archer
	g_modelObjects["Arrow"].modelMatrix = glm::mat4(1.0f);
	g_modelObjects["Arrow"].modelMatrix = glm::translate(g_modelObjects["Archer"].modelMatrix, glm::vec3(g_arrowX, g_arrowY, 0.0f));
	g_modelObjects["Arrow"].modelMatrix = glm::rotate(g_modelObjects["Arrow"].modelMatrix, glm::radians(g_arrowRotate), glm::vec3(0.0f, 1.0f, 0.0f));
	g_modelObjects["Arrow"].modelMatrix = glm::scale(g_modelObjects["Arrow"].modelMatrix, glm::vec3(g_arrowScale, g_arrowScale, 0.0f));

	// Archer transformations
	g_modelObjects["Archer"].modelMatrix = glm::mat4(1.0f);
	g_modelObjects["Archer"].modelMatrix = glm::translate(g_modelObjects["Archer"].modelMatrix, glm::vec3(g_archerX, g_archerY, 0.0f));
	g_modelObjects["Archer"].modelMatrix = glm::rotate(g_modelObjects["Archer"].modelMatrix, glm::radians(g_archerRotate), glm::vec3(0.0f, 1.0f, 0.0f));
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
	glBindTexture(GL_TEXTURE_2D, modelObject.modelTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void shutdown() {
	SDL_Quit();
}

