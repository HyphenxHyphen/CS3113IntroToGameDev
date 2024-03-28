#pragma once
#include "Entity.h"
class Player : public Entity
{
	void update(std::vector<GameMap>& levelMaps, std::map<std::string, Entity*>& allEnemies, float deltaTime);
};

