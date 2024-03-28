#pragma once
#include "ShaderProgram.h"

class Renderable
{
public:
	float m_elevation = 0.0f;

	virtual void render(ShaderProgram* program);
};

