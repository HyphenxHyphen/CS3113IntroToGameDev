#pragma once
#include "ScriptableScript.h"
#include "Entity.h"

class MovementController : public ScriptableScript
{
private:
	Entity* m_target = nullptr;
public:

	MovementController() {};
	MovementController() {};
	void update(float deltaTime);

	void willCollide(Entity* entity, GameMap* gameMap, bool pit, float deltaTime);
	void fall(Entity* entity, float elevationTarget);
	void diesOfCringe(Entity* entity, float deltaTime);

	// ————— SETTERS ————— //
	//void const setVars(std::vector<GameMap>* levelMapsIn, std::vector<Effect>* allEffectsIn, float deltaTimeIn) { levelMaps = levelMapsIn; allEffects = allEffectsIn; deltaTime = deltaTimeIn; };
};

