#pragma once
#include "Scene.h"

class Scene3 : public Scene
{
private:
	float gameEndTimer = 0.0f;
public:
	Scene3(GameGlobal* gameGlobal) :Scene(gameGlobal) {};

	// ————— METHODS ————— //
	void initialise() override;
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
