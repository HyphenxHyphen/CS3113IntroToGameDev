/**
* Author: John Li
* Assignment: Rise of the AI
* Date due: 2024-03-30, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "ScriptableObject.h"

ScriptableObject::~ScriptableObject()
{
	for (auto script : m_scripts)
	{
		delete script;
	}
	m_scripts.clear();
}


void ScriptableObject::processInput() {
	for (ScriptableScript* script : m_scripts) {
		script->processInput();
	}
}

void ScriptableObject::update(float deltaTime) {
	for (ScriptableScript* script : m_scripts) {
		script->update(deltaTime);
	}
}

void ScriptableObject::render(ShaderProgram* program) {
	for (ScriptableScript* script : m_scripts) {
		script->render(program);
	}
}
