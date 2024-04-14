#pragma once
#include "Scene.h"

class Scene1 : public Scene
{
	// ————— CONSTRUCTOR ————— //
	~Scene1();

	// ————— METHODS ————— //
	void initialise() override;
	void update(float delta_time) override;
	void render(ShaderProgram* program) override;
};

