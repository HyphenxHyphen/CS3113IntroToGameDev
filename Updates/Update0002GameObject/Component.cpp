#include "Component.h"

void Component::processInput() {
	//std::cout << "If you see this message, then something went wrong. This object does not processInput." << std::endl;
}

void Component::update(float deltaTime) {
	//std::cout << "If you see this message, then something went wrong. This object does not have an update function." << std::endl;
}

void Component::render(ShaderProgram* program) {
	//std::cout << "If you see this message, then something went wrong. This is a renderable object but it isn't a child class." << std::endl;
}