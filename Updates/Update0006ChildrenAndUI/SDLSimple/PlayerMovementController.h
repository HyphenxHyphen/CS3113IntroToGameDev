#pragma once
#include <SDL.h>
#include "Component.h"

class PlayerMovementController : public Component
{
public:
	PlayerMovementController(GameObject* parent) :Component(parent) {};
	void processInput();
	void update(float deltaTime) {};
};

