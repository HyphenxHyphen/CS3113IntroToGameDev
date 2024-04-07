#pragma once
#include "ShaderProgram.h"
#include "ScriptableScript.h"

#include <vector>
#include <map>

enum XYZ {X, Y, Z};

class ScriptableObject
{
private:
	std::string m_objectID = "";
	glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };
	glm::vec3 m_collisionSize = { 1.0f, 1.0f, 1.0f };
	std::vector<ScriptableScript*> m_scripts = {};
	std::vector<ScriptableObject>* m_allGameObjects;


public:
	ScriptableObject() {};
	ScriptableObject(std::string objectID, std::vector<ScriptableObject>* allGameObjects) { m_objectID = objectID; m_allGameObjects = allGameObjects; };
	~ScriptableObject();

	void processInput();
	void update(float deltaTime);
	void render(ShaderProgram* program);

	// ————— GETTERS ————— //
	std::string const getObjectID() const { return m_objectID; };
	glm::vec3 const getPosition() const { return m_position; };
	glm::vec3 const getRotation() const { return m_rotation; };
	glm::vec3 const getScale() const { return m_scale; };
	glm::vec3 const getCollisionSize() const { return m_collisionSize; };
	std::vector<ScriptableObject>* const getAllGameObjects() const { return m_allGameObjects; };
	template<class T>
	T* getComponent() {
		if (T * derived = dynamic_cast<T*>(this))
		{
			return derived;
		}
		for (auto scriptPointer : m_scripts) {
			if (T * derived = dynamic_cast<T*>(scriptPointer))
			{
				return derived;
			}
		}
		return nullptr;
	};

	// ————— SETTERS ————— //
	void const setObjectID(std::string newObjectID) { m_objectID = newObjectID; };
	void const setPosition(glm::vec3 newPosition) { m_position = newPosition; };
	void const setPosition(XYZ xyz, float newVal) { m_position[xyz] = newVal; };
	void const setRotation(glm::vec3 newRotation) { m_rotation = newRotation; };
	void const setScale(glm::vec3 newScale) { m_scale = newScale; m_collisionSize = { m_collisionSize.x * newScale.x, m_collisionSize.y * newScale.y, m_collisionSize.z * newScale.z }; };
	void const setCollisionSize(glm::vec3 newCollisionSize) { m_collisionSize = newCollisionSize; };
	template<class T> // T must be a ScriptableScript
	T* const addComponent() { T* t = new T(); t->setParent(this); m_scripts.push_back(t); return t; };
	template<class T>
	T* const addComponent(T* component) { component->setParent(this); m_scripts.push_back(t); return t; };
};

