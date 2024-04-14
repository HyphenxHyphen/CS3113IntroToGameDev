#pragma once
#include "Component.h"

enum RigidbodyType2D { DYNAMIC, KINEMATIC, STATIC };

struct CollisionData {
	bool collided = false;
	glm::vec3 normal = { 0.0f, 0.0f, 0.0f };
	float depth = std::numeric_limits<float>::max();
};

class Collider2D;

class Rigidbody2D : public Component
{
private:
	glm::vec3 m_velocity = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_acceleration = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_force = { 0.0f, 0.0f, 0.0f };
	float m_drag = 0.0f;
	float m_weight = 1.0f;
	float m_mass = 1.0f;
	bool m_useGravity = false;
	RigidbodyType2D m_rbType = DYNAMIC;

	std::vector<Collider2D*> m_colliders;
	std::vector<Rigidbody2D*> m_collisions;

	std::vector<std::string> m_ignoreTags = {};
	// Stuff to add

	//float angularDrag;
	//float angularVelocity;
	//unsigned int attachedColliderCount;
	
public:
	Rigidbody2D(GameObject* parent) :Component(parent) {};

	void update(float deltaTime);
	CollisionData isCollidingWith(Rigidbody2D* otherRB);
	void checkCollisions();
	void cleanCollisions();
	bool ignoreCollisionTagsCheck(GameObject* gameObjectOther);

	// ————— GETTERS ————— //
	glm::vec3 const getVelocity() const { return m_velocity; };
	glm::vec3 const getAcceleration() const { return m_acceleration; };
	glm::vec3 const getForce() const { return m_force; };
	float const getDrag() const { return m_drag; };
	bool const getUseGravity() const { return m_useGravity; };
	RigidbodyType2D const getRBType() const { return m_rbType; };
	std::vector<Collider2D*> const getColliders() const { return m_colliders; };
	std::vector<std::string> getIgnoreTags() { return m_ignoreTags; };
	bool hasIgnoreTag(std::string ignoreTag) { return find(m_ignoreTags.begin(), m_ignoreTags.end(), ignoreTag) != m_ignoreTags.end(); };

	// ————— SETTERS ————— //
	void const setVelocity(glm::vec3 newVelocity) { m_velocity = newVelocity; };
	void const setVelocity(XYZ xyz, float newVal) { m_velocity[xyz] = newVal; };
	void const addVelocity(glm::vec3 newVelocity) { m_velocity += newVelocity; };
	void const addVelocity(XYZ xyz, float newVal) { m_velocity[xyz] += newVal; };

	void const setAcceleration(glm::vec3 newAcceleration) { m_acceleration = newAcceleration; };
	void const setAcceleration(XYZ xyz, float newVal) { m_acceleration[xyz] = newVal; };
	void const addAcceleration(glm::vec3 newAcceleration) { m_acceleration += newAcceleration; };
	void const addAcceleration(XYZ xyz, float newVal) { m_acceleration[xyz] += newVal; };

	void const setForce(glm::vec3 force) { m_force = force; };
	void const setForce(XYZ xyz, float newVal) { m_acceleration[xyz] = newVal; };
	void const addForce(glm::vec3 newAcceleration) { m_acceleration += newAcceleration; };
	void const addForce(XYZ xyz, float newVal) { m_acceleration[xyz] += newVal; };

	void const setDrag(float drag) { m_drag = drag; };
	void const setUseGravity(bool useGravity) { m_useGravity = useGravity; };
	void const setRBType(RigidbodyType2D rbType) { m_rbType = rbType; };

	void const addCollider(Collider2D* collider) { m_colliders.push_back(collider); };
	void const removeCollider(Collider2D* collider);
	void const addIgnoreTag(std::string tag) { m_ignoreTags.push_back(tag); };
	void const removeIgnoreTag(std::string tag) { if (hasIgnoreTag(tag)) { m_ignoreTags.erase(find(m_ignoreTags.begin(), m_ignoreTags.end(), tag)); } }

};

