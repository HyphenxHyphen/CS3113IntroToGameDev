#pragma once
#include "Scene.h"

class Scene2 : public Scene {
private:
	std::vector<EntityController*> m_allies;
	std::vector<EntityController*> m_enemies;
	std::vector<EntityStatsHoverController*> m_allHoverControllers;

	HexData m_allyHexData;
	HexData m_enemyHexData;

	std::vector<EntityMoveDisplayController*> m_moveDisplays = {};
	EntityTurnOrderDisplayController* m_turnOrderDisplay = nullptr;
	//bool m_targeting = false;
	EntityMoveDisplayController* m_usingMove = nullptr;

	float m_timer = 0.0f;
	bool m_override = false;
	GameObject* battleEndScreen = nullptr;
	
public:
	Scene2(GameGlobal* gameGlobal) :Scene(gameGlobal) { initialise(); };
	~Scene2() { clearScene(); };

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
