#include "Scene.h"

Scene::~Scene() {
	for (auto gameObject : m_gameState.allGameObjects)
	{
		delete gameObject;
	}
	m_gameState.allGameObjects.clear();
	SDL_Quit();
}