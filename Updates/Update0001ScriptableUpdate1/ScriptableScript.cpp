#include "ScriptableScript.h"

void ScriptableScript::processInput() {
	//std::cout << "If you see this message, then something went wrong. This object does not processInput." << std::endl;
}

void ScriptableScript::update(float deltaTime) {
	//std::cout << "If you see this message, then something went wrong. This object does not have an update function." << std::endl;
}

void ScriptableScript::render(ShaderProgram* program) {
	//std::cout << "If you see this message, then something went wrong. This is a renderable object but it isn't a child class." << std::endl;
}