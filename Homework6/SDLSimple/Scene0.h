#pragma once
#include "Scene.h"

class Scene0 : public Scene {
private:
	float seconds = 0.0f;
public:
	Scene0(GameGlobal* gameGlobal) :Scene(gameGlobal) { initialise(); }
	~Scene0() { clearScene(); };

	// ————— METHODS ————— //
	void initialise() override;
	void load() override {};
	void processInput(bool* gameIsRunning) override;
	void update(float deltaTime) override;
	void render(ShaderProgram* program) override;
	void clearScene() override;
};

class StartGameButton : public Button {
private:
	std::vector<GameObject*>* g_ants = nullptr;
	int m_antId = 0;
	Text* m_text = nullptr;
	glm::vec3 m_normalColor = { 0, 0, 0 };
	glm::vec3 m_hoveredColor = { 0, 0, 0 };
	int* m_nextScene = nullptr;

public:
	StartGameButton(GameObject* parent, std::vector<Button*>* allButtons, SlicedImageRenderer* image, Camera* camera, std::vector<GameObject*>* ants, Text* text, int antId, glm::vec3 normalColor, glm::vec3 hoveredColor, int* nextScene) :Button(parent, allButtons, image, camera) { g_ants = ants; m_antId = antId; m_text = text; m_normalColor = normalColor; m_hoveredColor = hoveredColor; m_nextScene = nextScene; };
	void update(float deltaTime);
	void processInput() override {};
	void press() override;
};