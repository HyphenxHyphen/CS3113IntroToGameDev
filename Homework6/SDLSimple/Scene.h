#pragma once
// Scene.h
#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <ctime>
#include "ShaderProgram.h"
#include "Util.h"
#include "GameObject.h"
#include "Component.h"
#include "SpriteRenderer.h"
#include "Rigidbody2D.h"
#include "Collider2D.h"
#include "PolygonCollider2D.h"
#include "CircleCollider2D.h"
#include "TileMap.h"
#include "EntityController.h"
#include "ScreenTransitionFX.h"
#include "Canvas.h"
#include "Text.h"
#include "SlicedImageRenderer.h"
#include "Camera.h"
#include "Button.h"
#include "RotateLeftButton.h"
#include "RotateRightButton.h"
#include "LineRenderer.h"
#include "NodeController.h"
#include "HexController.h"
#include "EntityInfoDisplayController.h"
#include "EntityInfoHoverController.h"

enum GameMode { MAIN_MENU, GAME_LEVEL, GAME_OVER, GAME_PAUSED };

struct GameState {
	Font font;
	GameMode gameMode = GAME_LEVEL;
	bool victory = false;
	std::vector<GameObject*> allGameObjects = {};
	std::vector<Button*> allButtons = {};
	GameObject* findGameObject(std::string objectID) {
		for (GameObject* gameObject : allGameObjects) {
			if (gameObject->getObjectID().find(objectID) != std::string::npos) {
				return gameObject;
			}
		}
		return nullptr;
	}
};

struct GameGlobal {
	std::vector<GameObject*> g_activeNodes = {};
	std::vector<GameObject*> g_ants;
	int g_scrap;
	
	//TODO: SCRAP
	//TODO: TREASURE

	std::map<std::string, Mix_Chunk*> g_sfx = {};
};

class Scene {
public:
	// Transition FX
	//ScreenTransitionFX* g_tFX;

	// Game Globals
	GameGlobal* g_gameGlobal;

	// Game State
	GameState m_gameState;

	// Game Camera
	Camera m_camera;

	// Next Scene
	int m_nextScene = -1;

	// Shader Globals
	glm::mat4 m_viewMatrix = glm::mat4(0.0f);        // Defines camera location and orientation
	glm::mat4 m_projectionMatrix = glm::mat4(0.0f);

	// ————— CONSTRUCTOR ————— //
	Scene(GameGlobal* gameGlobal) { g_gameGlobal = gameGlobal; }
	~Scene();

	// ————— METHODS ————— //
	virtual void initialise() = 0;
	virtual void load() = 0;
	virtual void processInput(bool* gameIsRunning) = 0;
	virtual void update(float delta_time) = 0;
	virtual void render(ShaderProgram* program) = 0;
	virtual void clearScene() = 0;
	int getSwitchScene() { return m_nextScene; };
	void switchScene(unsigned int nextScene) { m_nextScene = nextScene; };
	std::vector<GameObject*> recursiveRenderChildrenSort(GameObject* parent);
};

