#pragma once
#include "ScriptableObject.h"

class ScriptableScript
{
protected:
	ScriptableObject* m_parent = nullptr;
public:
	ScriptableScript() {};

	virtual void processInput();
	virtual void update(float deltaTime) = 0;
	virtual void render(ShaderProgram* program);

	// ————— GETTERS ————— //
	std::string const getObjectID() const { return m_parent->getObjectID(); };
	glm::vec3 const getPosition() const { return m_parent->getPosition(); };
	glm::vec3 const getRotation() const { return m_parent->getRotation(); };
	glm::vec3 const getScale() const { return m_parent->getScale(); };
	glm::vec3 const getCollisionSize() const { return m_parent->getCollisionSize; };
	ScriptableObject* const getParent() const { return m_parent; };
	std::vector<ScriptableObject>* const getAllGameObjects() const { return m_parent->getAllGameObjects(); };
	template<class T>
	T* getComponent() {
		return m_parent->getComponent<T>();
	};

	// ————— SETTERS ————— //
	void const setObjectID(std::string newObjectID) { m_parent->setObjectID(newObjectID); };
	void const setPosition(glm::vec3 newPosition) { m_parent->setPosition(newPosition); };
	void const setPosition(XYZ xyz, float newVal) { m_parent->setPosition(xyz, newVal); };
	void const setRotation(glm::vec3 newRotation) { m_parent->setRotation(newRotation); };
	void const setScale(glm::vec3 newScale) { m_parent->setScale(newScale); };
	void const setCollisionSize(glm::vec3 newCollisionSize) { m_parent->setCollisionSize(newCollisionSize); };
	void const setParent(ScriptableObject* newParent) { m_parent = newParent; }; // I don't think this is ever used
};

