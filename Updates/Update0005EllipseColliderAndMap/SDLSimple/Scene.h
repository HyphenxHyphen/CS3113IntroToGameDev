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
#include "PlayerMovementController.h"
#include "Collider2D.h"
#include "PolygonCollider2D.h"
#include "CircleCollider2D.h"
#include "TileMap.h"

struct GameState {
	//Entity* font;
	//GameMode gameMode = GAME_LEVEL;
	bool victory = false;
	std::vector<GameObject*> allGameObjects = {};
	glm::vec3 cameraPos = { 0.0f, 0.0f, 0.0f };

	GameObject* findGameObject(std::string objectID) {
		for (GameObject* gameObject : allGameObjects) {
			if (gameObject->getObjectID().find(objectID) != std::string::npos) {
				return gameObject;
			}
		}
		return nullptr;
	}
};

class Scene
{
public:
	// Window
	const int WINDOW_WIDTH = 640 * 2.0f;
	const int WINDOW_HEIGHT = 480 * 2.0f;

	// Camera
	const int VIEWPORT_X = 0;
	const int VIEWPORT_Y = 0;
	const int VIEWPORT_WIDTH = WINDOW_WIDTH;
	const int VIEWPORT_HEIGHT = WINDOW_HEIGHT;
	float cameraScale = 1.5f;

	// Game State
	GameState m_gameState;

	// Shader Globals
	glm::mat4 m_viewMatrix;        // Defines camera location and orientation
	glm::mat4 m_projectionMatrix;

	// ————— CONSTRUCTOR ————— //
	~Scene();

	// ————— METHODS ————— //
	virtual void initialise() = 0;
	virtual void update(float delta_time) = 0;
	virtual void render(ShaderProgram* program) = 0;

};

