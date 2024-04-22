#include "GameObject.h"
#include "Component.h"

GameObject::~GameObject() {
	for (int i = m_components.size() - 1; i >= 0; i--) {
		//std::cout << "Deleting component " << std::to_string(i) << " from " << getObjectID() << std::endl;
		delete m_components[i];
	}
	m_components.clear();

	for (int i = m_children.size() - 1; i >= 0; i--) {
		m_children[i]->setParent(nullptr);
		delete m_children[i];
	}
	m_children.clear();

	if (m_parent != nullptr) {
		m_parent->removeChild(this);
	}

	auto it = std::find(m_allGameObjects->begin(), m_allGameObjects->end(), this);
	if (it != m_allGameObjects->end()) {
		m_allGameObjects->at(it - m_allGameObjects->begin()) = nullptr;
	}

	//for (int i = 0; i < m_allGameObjects->size(); i++) {
	//	if ((&m_allGameObjects)[i] == this) {
    //
	//	}
	//	m_children[i]->setParent(nullptr);
	//	delete m_children[i];
	//}
}

void GameObject::processInput() {
	for (Component* component : m_components) {
		component->processInput();
	}
}

void GameObject::update(float deltaTime) {
	for (Component* component : m_components) {
		component->update(deltaTime);
	}

	for (GameObject* child : m_children) {
		child->setPosition(child->getPositionRelative() + getPosition());
		child->setRotation(child->getRotationRelative() + getRotation());
		child->setScale(child->getScaleRelative() * getScale());
	}

	m_modelMatrix = glm::mat4(1.0f);
	m_modelMatrix = glm::translate(m_modelMatrix, { getPosition().x,  getPosition().y, 0.0f });
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_modelMatrix = glm::scale(m_modelMatrix, getScale());
}

void const GameObject::setPosition(glm::vec3 newPosition) { 
	m_position = newPosition; 
	for (GameObject* child : m_children) {
		child->setPosition(child->getPositionRelative() + getPosition());
	}
}

void const GameObject::setPosition(XYZ xyz, float newVal) { 
	m_position[xyz] = newVal;
	for (GameObject* child : m_children) {
		child->setPosition(child->getPositionRelative() + getPosition());
	}
}

void const GameObject::setRotation(glm::vec3 newRotation) { 
	m_rotation = newRotation; 
	for (GameObject* child : m_children) {
		child->setRotation(child->getRotationRelative() + getRotation());
	}
}

void const GameObject::setRotation(XYZ xyz, float newVal) {
	m_rotation[xyz] = newVal; 
	for (GameObject* child : m_children) {
		child->setRotation(child->getRotationRelative() + getRotation());
	}
}

void const GameObject::setScale(glm::vec3 newScale) {
	m_scale = newScale; 
	for (GameObject* child : m_children) {
		child->setScale(child->getScaleRelative() * getScale());
	}
}

void GameObject::render(ShaderProgram* program) {
	//std::cout << "GameObject Render Called " << m_components.size() << std::endl;
	for (Component* component : m_components) {
		component->render(program);
	}
}

// Be VERY careful using this.
// This should only be used by Component's destructor.
void const GameObject::removeComponent(Component* component) {
	auto it = std::find(m_components.begin(), m_components.end(), component);
	if (it != m_components.end()) {

		m_components.erase(it);
	}
};

// Be VERY careful using this.
// This should only be used by Child's destructor.
void const GameObject::removeChild(GameObject* child) {
	auto it = std::find(m_children.begin(), m_children.end(), child);
	if (it != m_children.end()) {
		m_children.erase(it);
	}
};