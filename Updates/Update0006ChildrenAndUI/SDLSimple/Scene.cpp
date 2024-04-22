#include "Scene.h"

Scene::~Scene() {
	for (int i = m_gameState.allGameObjects.size() - 1; i >= 0; i--) {
		if (m_gameState.allGameObjects[i] != nullptr) {
			delete m_gameState.allGameObjects[i];
		}
	}
	m_gameState.allGameObjects.clear();
}