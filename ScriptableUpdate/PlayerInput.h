#pragma once

#include <SDL.h>
#include "Scriptable.h"
#include "Entity.h"

class PlayerInput : public Scriptable
{
public:
	void processInput();
};

