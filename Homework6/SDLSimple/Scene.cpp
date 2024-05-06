#include "Scene.h"

Scene::~Scene() {
	for (int i = m_gameState.allGameObjects.size() - 1; i >= 0; i--) {
		if (m_gameState.allGameObjects[i] != nullptr) {
			delete m_gameState.allGameObjects[i];
		}
	}
	m_gameState.allGameObjects.clear();
}

// It's just a depthfirst walk
std::vector<GameObject*> Scene::recursiveRenderChildrenSort(GameObject* parent) {
	std::vector<GameObject*> result = { parent };
	std::vector<GameObject*> sortedRender = *(parent->getChildren());

	for (unsigned int i = 1; i < sortedRender.size(); i++) {
		int j = i;
		while (j > 0 && ((sortedRender[j]->getPosition().z < sortedRender[j - 1]->getPosition().z) || (sortedRender[j]->getPosition().y > sortedRender[j - 1]->getPosition().y))) {
			std::swap(sortedRender[j], sortedRender[j - 1]);
			j--;
		}
	}

	for (GameObject* child : sortedRender) {
		if (child->isAlive()) {
			std::vector<GameObject*> childResult = recursiveRenderChildrenSort(child);
			result.insert(result.end(), childResult.begin(), childResult.end());
		}
	}
	return result;
}