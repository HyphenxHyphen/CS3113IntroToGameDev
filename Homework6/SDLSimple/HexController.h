#pragma once
#include "Component.h"
#include "EntityController.h"

class HexController : public Component {
private:
	EntityController* m_occupant = nullptr;
public:
	HexController(GameObject* parent) :Component(parent) {};

	void update(float deltaTime) {};

	void static rotateHexes(HexData* hexData, bool clockwise, int amount);

	// ————— GETTERS ————— //
	EntityController* const getOccupant() const { return m_occupant; };

	// ————— SETTERS ————— //
	void const setOccupant(EntityController* occupant) { m_occupant = occupant; };
};

