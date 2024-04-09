/**
* Author: John Li
* Assignment: Rise of the AI
* Date due: 2024-03-30, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "GameObject.h"
#include "Component.h"

GameObject::~GameObject()
{
	for (auto script : m_components)
	{
		delete script;
	}
	m_components.clear();
}


void GameObject::processInput() {
	for (Component* script : m_components) {
		script->processInput();
	}
}

void GameObject::update(float deltaTime) {
	for (Component* script : m_components) {
		script->update(deltaTime);
	}

	m_modelMatrix = glm::mat4(1.0f);
	m_modelMatrix = glm::translate(m_modelMatrix, { getPosition().x,  getPosition().y, 0.0f });
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_modelMatrix = glm::scale(m_modelMatrix, getScale());
}

void GameObject::render(ShaderProgram* program) {
	//std::cout << "GameObject Render Called " << m_components.size() << std::endl;

	for (Component* script : m_components) {
		script->render(program);
	}
}
