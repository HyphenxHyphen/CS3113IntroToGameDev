#pragma once
#include "Scene.h"

class Scene0 : public Scene {
private:
	float seconds = 0.0f;
public:
	Scene0(GameGlobal* gameGlobal) :Scene(gameGlobal) {};

	// ————— METHODS ————— //
	void initialise() override;
	void processInput(bool* gameIsRunning) override;
	void update(float deltaTime) override;
	void render(ShaderProgram* program) override;
	void clearScene() override;
};

