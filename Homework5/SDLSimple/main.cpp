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
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <vector>
#include <ctime>
#include "Scene.h"
#include "Scene0.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"

// Window
const float WINDOW_WIDTH = 640 * 2.0f;
const float WINDOW_HEIGHT = 480 * 2.0f;

// BG
float g_bgR =  71.0f / 255.0f;
float g_bgG = 171.0f / 255.0f;
float g_bgB = 171.0f / 255.0f;
float g_bgA = 1.0f;

// Camera
const int VIEWPORT_X = 0;
const int VIEWPORT_Y = 0;
const float VIEWPORT_WIDTH = WINDOW_WIDTH;
const float VIEWPORT_HEIGHT = WINDOW_HEIGHT;
float cameraScale = 1.5f;

// Shader filepaths
const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl";
const char F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

// Display Globals
SDL_Window* g_displayWindow;

// Audio
const int CD_QUAL_FREQ = 44100;  // compact disk (CD) quality frequency
const int AUDIO_CHAN_AMT = 2;
const int AUDIO_BUFF_SIZE = 4096;

// Game Globals
Scene* g_gameScene;
std::vector<Scene*> g_gameScenes;
int switchSceneFlag = -1;
GameGlobal g_gameGlobal;
Mix_Music* g_music;
ScreenTransitionFX* g_tfx;

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
void switchScene(int sceneInd);
void processInput();
void update();
void updateDeltaTime();
void render();
void shutdown();

int main(int argc, char* argv[]) {
	initialize();
	while (g_gameIsRunning) {
		processInput();
		update();
		if (g_gameScene->getSwitchScene() >= 0) {
			switchSceneFlag = g_gameScene->getSwitchScene();
			g_gameScene->switchScene(-1);
			g_tfx->start(FADEOUT, 0.5f);
			//std::cout << "TFX START " << g_tfx->m_currentEffect << std::endl;
			//std::cout << "TFX RUNNING " << g_tfx->isRunning() << std::endl;
		}
		if (switchSceneFlag >= 0) {
			if (!g_tfx->isRunning()) {
				//std::cout << "TFX RUNNING? " << g_tfx->isRunning() << std::endl;

				g_gameScene->clearScene();
				switchScene(switchSceneFlag);
				switchSceneFlag = -1;
			}
		}

		render();
	}
	shutdown();
	return 0;
}

void switchScene(int sceneInd ) {
	g_tfx->start(FADEIN, 0.5f);
	g_gameScene = g_gameScenes[sceneInd];
	g_gameScene->initialise();
}

void initialize()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	g_displayWindow = SDL_CreateWindow("Project Go!",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		(int)WINDOW_WIDTH, (int)WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL);

	SDL_GLContext context = SDL_GL_CreateContext(g_displayWindow);
	SDL_GL_MakeCurrent(g_displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, (int)VIEWPORT_HEIGHT);

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

	// Initialize Audio

	Mix_OpenAudio(
		CD_QUAL_FREQ,        // the frequency to playback audio at (in Hz)
		MIX_DEFAULT_FORMAT,  // audio format
		AUDIO_CHAN_AMT,      // number of channels (1 is mono, 2 is stereo, etc).
		AUDIO_BUFF_SIZE      // audio buffer size in sample FRAMES (total samples divided by channel count)
	);
	g_music = Mix_LoadMUS("Assets/Audio/Dungeon1Looped.mp3");
	Mix_PlayMusic(g_music, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME * 25/100);

	// Initialize Game State

	g_tfx = new ScreenTransitionFX(g_projectionMatrix, g_viewMatrix);

	g_gameGlobal = GameGlobal();
	g_gameGlobal.playerLives = 3;

	g_gameGlobal.g_jumpSFX = Mix_LoadWAV("Assets/Audio/JumpSFX.wav");
	Mix_VolumeChunk(
		g_gameGlobal.g_jumpSFX,
		MIX_MAX_VOLUME * 50 / 100 
	);

	g_gameGlobal.g_atkSFX = Mix_LoadWAV("Assets/Audio/AtkSFX.wav");
	Mix_VolumeChunk(
		g_gameGlobal.g_atkSFX,
		MIX_MAX_VOLUME * 50 / 100
	);

	g_gameGlobal.g_hitSFX = Mix_LoadWAV("Assets/Audio/HitSFX.wav");
	Mix_VolumeChunk(
		g_gameGlobal.g_hitSFX,
		MIX_MAX_VOLUME * 50 / 100
	);

	g_gameGlobal.g_dieSFX = Mix_LoadWAV("Assets/Audio/DieSFX.wav");
	Mix_VolumeChunk(
		g_gameGlobal.g_dieSFX,
		MIX_MAX_VOLUME * 50 / 100
	);

	g_gameScenes.push_back(new Scene0(&g_gameGlobal));
	g_gameScenes.push_back(new Scene1(&g_gameGlobal));
	g_gameScenes.push_back(new Scene2(&g_gameGlobal));
	g_gameScenes.push_back(new Scene3(&g_gameGlobal));
	switchScene(0);
}

void processInput()
{
	if (g_tfx->isRunning()) return;

	// Process Game Scene Input
	g_gameScene->processInput(&g_gameIsRunning);
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
		// Update Transition
		g_tfx->update(FIXED_TIMESTEP);

		// Update Scene
		g_gameScene->update(FIXED_TIMESTEP);

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

void render() {
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(g_program.get_program_id());
	g_gameScene->render(&g_program);
	g_tfx->render();

	SDL_GL_SwapWindow(g_displayWindow);
}

void shutdown() {
	for (auto gameScene : g_gameScenes)
	{
		delete gameScene;
	}
	g_gameScenes.clear();
	g_gameScene = nullptr;
	Mix_FreeChunk(g_gameGlobal.g_jumpSFX);
	Mix_FreeChunk(g_gameGlobal.g_hitSFX);
	Mix_FreeChunk(g_gameGlobal.g_atkSFX);
	Mix_FreeChunk(g_gameGlobal.g_dieSFX);
	Mix_FreeMusic(g_music);
	SDL_Quit();
}
