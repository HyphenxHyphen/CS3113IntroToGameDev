#pragma once
#include "Scene.h"

class Scene1 : public Scene {
private:
	std::vector<GameObject*> m_nodes = {};
	int m_mapHeight = 15;
	int m_mapWidth = 7;
	 
	NodeType m_chosenNode = NODENONE;
	int m_chosenNodeId = -1;
	std::vector<GameObject*> m_startingNodes = {};

public:
	Scene1(GameGlobal* gameGlobal) :Scene(gameGlobal) { initialise(); };
	~Scene1() { clearScene(); };

	// ————— METHODS ————— //
	void initialise() override;
	void load() override;
	void unload();
	void processInput(bool* gameIsRunning) override;
	void processInputMainMenu(bool* gameIsRunning);
	void processInputGameLevel(bool* gameIsRunning);
	void processInputGameOver(bool* gameIsRunning);
	void update(float deltaTime) override;
	void updateMainMenu(float deltaTime);
	void updateGameLevel(float deltaTime);
	void updateGameOver(float deltaTime);
	void render(ShaderProgram* program) override;
	void clearScene() override;
};

