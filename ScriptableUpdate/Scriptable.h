#pragma once
#include "ShaderProgram.h"
#include <map>
#include <vector>

class Scriptable
{
protected:
	glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };
	glm::vec3 m_collisionSize = { 1.0f, 1.0f, 1.0f };
	//std::map<std::string, Scriptable> m_scripts = {};
	std::vector<Scriptable*> m_scripts = {};
	Scriptable* m_parent = nullptr;

public:
	~Scriptable();
	virtual void processInput();
	virtual void update();
	virtual void render(ShaderProgram* program);

	// ————— GETTERS ————— //
	glm::vec3 const getPosition() const { return m_position; };
	glm::vec3 const getRotation() const { return m_rotation; };
	glm::vec3 const getScale() const { return m_scale; };
	glm::vec3 const getCollisionSize() const { return m_collisionSize; };
	Scriptable* const getParent() const { return m_parent; };
	template<class T>
	T* getComponent() {
		if (T * derived = dynamic_cast<T*>(this))
		{
			return derived;
		}
		for (auto scriptPointer : m_scripts) {
			if (T* derived = dynamic_cast<T*>(scriptPointer))
			{
				return derived;
			}
		}
		return nullptr;
	};

	// ————— SETTERS ————— //
	void const setPosition(glm::vec3 newPosition) { m_position = newPosition; };
	void const setRotation(glm::vec3 newRotation) { m_rotation = newRotation; };
	void const setScale(glm::vec3 newScale) { m_scale = newScale; m_collisionSize = { m_collisionSize.x * newScale.x, m_collisionSize.y * newScale.y, m_collisionSize.z * newScale.z }; };
	void const setCollisionSize(glm::vec3 newCollisionSize) { m_collisionSize = newCollisionSize; };
	void const setParent(Scriptable* newParent) { m_parent = newParent; };
	template<class T>
	T* const addComponent() { T* t = new T(); t->setParent(this); m_scripts.push_back(t); return t; };
};

