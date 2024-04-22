#pragma once
#include "ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

enum XYZ { X, Y, Z };

class Component;

class GameObject {
private:
	std::string m_objectID = "";
	glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };
	glm::vec3 m_positionRelative = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_rotationRelative = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_scaleRelative = { 1.0f, 1.0f, 1.0f };
	//glm::vec3 m_collisionSize = { 1.0f, 1.0f, 1.0f };
	std::vector<GameObject*>* m_allGameObjects;
	std::vector<Component*> m_components = {};
	glm::mat4 m_modelMatrix = glm::mat4(1.0f);
	bool m_isAlive = true;
	std::vector<std::string> m_tags = {};
	GameObject* m_parent;
	std::vector<GameObject*> m_children = {};

public:
	GameObject(std::string objectID, std::vector<GameObject*>* allGameObjects) { m_objectID = objectID; m_allGameObjects = allGameObjects; allGameObjects->push_back(this); };
	~GameObject();

	void processInput();
	void update(float deltaTime);
	void render(ShaderProgram* program);

	// ————— GETTERS ————— //
	bool const isAlive() const { return m_isAlive; };
    glm::mat4 const getModelMatrix() const { return m_modelMatrix; };
	std::string const getObjectID() const { return m_objectID; };
	glm::vec3 const getPosition() const { return m_position; };
	glm::vec3 const getRotation() const { return m_rotation; };
	glm::vec3 const getScale() const { return m_scale; };
	glm::vec3 const getPositionRelative() const { return m_positionRelative; };
	glm::vec3 const getRotationRelative() const { return m_rotationRelative; };
	glm::vec3 const getScaleRelative() const { return m_scaleRelative; };
	//glm::vec3 const getCollisionSize() const { return m_collisionSize; };
	std::vector<GameObject*>* const getAllGameObjects() const { return m_allGameObjects; };
	template<class T>
	T* getComponent() {
		for (auto component : m_components) {
			if (T* derived = dynamic_cast<T*>(component)) {
				return derived;
			}
		}
		return nullptr;
	};
	int const getComponentsSize() const { return m_components.size(); };
	std::vector<std::string> const getTags() const { return m_tags; };
	bool const hasTag(std::string tag) const { return find(m_tags.begin(), m_tags.end(), tag) != m_tags.end(); };
	std::vector<GameObject*>* getChildren() { return &m_children; };
	GameObject* const getChild(std::string objectID) const {
		for (auto child : m_children) {
			if (child->getObjectID() == objectID) {
				return child;
			}
		}
		return nullptr;
	}

	// ————— SETTERS ————— //
	void const setAlive(bool isAlive) { m_isAlive = isAlive; };
	void const setModelMatrix(glm::mat4 modelMatrix) { m_modelMatrix = modelMatrix; };
	void const setObjectID(std::string newObjectID) { m_objectID = newObjectID; };
	void const setPosition(glm::vec3 newPosition);
	void const setPosition(XYZ xyz, float newVal);
	void const setRotation(glm::vec3 newRotation);
	void const setRotation(XYZ xyz, float newVal);
	void const setScale(glm::vec3 newScale); 
	void const setPositionRelative(glm::vec3 newPosition) { m_positionRelative = newPosition; };
	void const setPositionRelative(XYZ xyz, float newVal) { m_positionRelative[xyz] = newVal; };
	void const setRotationRelative(glm::vec3 newRotation) { m_rotationRelative = newRotation; };
	void const setRotationRelative(XYZ xyz, float newVal) { m_rotationRelative[xyz] = newVal; };
	void const setScaleRelative(glm::vec3 newScale) { m_scaleRelative = newScale; };
	template<class T> // T must be a Component
	T* const addComponent() { T* t = new T(this); m_components.push_back(t); return t; };
	template<class T>
	T* const addComponent(T* component) { component->setParent(this); m_components.push_back(component); return component; };
	void const removeComponent(Component* component);
	void const addTag(std::string tag) { m_tags.push_back(tag); };
	void const removeTag(std::string tag) { if (hasTag(tag)) { m_tags.erase(find(m_tags.begin(), m_tags.end(), tag)); } }
	void const setParent(GameObject* parent) { m_parent = parent; }
	void const addChild(GameObject* child) { m_children.push_back(child); child->setParent(this); };
	void const removeChild(GameObject* child);
};

