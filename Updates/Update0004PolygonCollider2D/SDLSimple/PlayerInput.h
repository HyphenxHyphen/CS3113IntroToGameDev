#pragma once

#include <SDL.h>
#include "Component.h"
#include "Entity.h"

class PlayerInput : public Component
{
public:
	PlayerInput(GameObject* parent) :Component(parent) {};
	void processInput();
};

