#pragma once

#include <SDL.h>
#include "GameObject.h"
#include "Entity.h"

class PlayerInput : public Component
{
public:
	void processInput();
};

