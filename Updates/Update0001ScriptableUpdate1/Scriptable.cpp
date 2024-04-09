/**
* Author: John Li
* Assignment: Rise of the AI
* Date due: 2024-03-30, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Scriptable.h"

Scriptable::~Scriptable()
{
	for (auto script : m_scripts)
	{
		if (script != this) {
			delete script;
		}
	}
	m_scripts.clear();
}

void Scriptable::processInput() {
	//std::cout << "If you see this message, then something went wrong. This object does not processInput." << std::endl;
}

void Scriptable::update() {
	//std::cout << "If you see this message, then something went wrong. This object does not have an update function." << std::endl;
}

void Scriptable::render(ShaderProgram* program) {
	//std::cout << "If you see this message, then something went wrong. This is a renderable object but it isn't a child class." << std::endl;
}