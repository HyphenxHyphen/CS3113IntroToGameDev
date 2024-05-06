#pragma once
#include <SDL_mixer.h>
#include "Component.h"
#include "Rigidbody2D.h"
#include "SpriteRenderer.h"
#include "TileMap.h"
#include "UnitMove.h"
#include "Stack.h"

enum AnimState { IDLE, TURNSTART, TURNEND, ATTACK, HIT };
//enum FaceDirection { UP, DOWN, LEFT, RIGHT };

class HexController;
class EntityTurnOrderDisplayController;

struct HexData {
	std::vector<HexController*> m_hexes = {};
	int m_rotationDelta = 0;
	int m_rotations = 0;
};

class EntityController : public Component {
private:
	int m_id = 0;
	std::string m_name = "";
	std::string m_species = "";
	std::string m_commonName = "";
	GLuint m_sprite = 0;
	GLuint m_icon = 0;
	int m_spawnHex = 0;

	std::map<std::string, float> m_stats = 
	{   {"dna",      	  0.0f},
		{"feast",    	  0.0f},
		{"feastMax", 	  0.0f},
		{"hpScore",  	  0.0f},
		{"atkScore", 	  0.0f},
		{"defScore", 	  0.0f},
		{"accScore", 	  0.0f},
		{"spdScore", 	  0.0f},
		{"hpMul",    	  0.0f},
		{"atkMul",   	  0.0f},
		{"defMul",   	  0.0f},
		{"accMul",   	  0.0f},
		{"spdMul",   	  0.0f},
		{"hp",       	  0.0f},
		{"hpMax",    	  0.0f},
		{"atk",      	  0.0f},
		{"def",      	  0.0f},
		{"acc",      	  0.0f},
		{"spd",			  0.0f},
		{"dodge",		  0.0f},
		{"crit",		  0.0f},
		{"dmg",			  1.0f},
		{"hpDelta",       0.0f},
		{"hpMaxDelta",    0.0f},
		{"atkDelta",      0.0f},
		{"defDelta",      0.0f},
		{"accDelta",      0.0f},
		{"spdDelta",      0.0f},
		{"dodgeDelta",    0.0f},
		{"critDelta",     0.0f},
		{"dmgDelta",	  0.0f},
	};

	int m_hex = 0;

	//TODO Status
	std::vector<Stack*> m_stacks = {};

	//TODO Moves
	std::vector<int> m_movePool = { 0 };
	std::vector<UnitMoveInfo> m_movesKnown = {};

	bool m_isAnt = false;
	std::vector<EntityController*>* m_allies = nullptr;
	std::vector<EntityController*>* m_enemies = nullptr;
	EntityTurnOrderDisplayController* m_turnOrderDisplay = nullptr;

	HexData* m_allyHexData = nullptr;
	HexData* m_enemyHexData = nullptr;

	std::map<std::string, Mix_Chunk*> m_sfx = {};
	AnimState m_animState = IDLE;
	float m_turnWait = 0.0f;

public:
	EntityController(GameObject* parent, int id, bool isAnt, std::vector<EntityController*>* allies, std::vector<EntityController*>* enemies, HexData* allyHexData, HexData* enemyHexData, EntityTurnOrderDisplayController* turnOrderDisplay);
	EntityController(GameObject* parent, EntityController* original, std::vector<EntityController*>* allies, std::vector<EntityController*>* enemies, HexData* allyHexData, HexData* enemyHexData, EntityTurnOrderDisplayController* turnOrderDisplay);

	void update(float deltaTime);
	void updateStats();
	void getDefault();
	void build();
	void loadStats(EntityController* other);
	void diesOfCringe(float deltaTime);
	void onTurnStart();
	void onTurnEnd();

	// ————— GETTERS ————— //
	float const getStat(std::string stat) { return m_stats[stat]; };
	std::map<std::string, float>* const getStats() { return &m_stats; };
	std::vector<UnitMoveInfo> const getUnitMovesKnown() const { return m_movesKnown; };
	bool const isAnt() const { return m_isAnt; };
	AnimState const getAnimState() const { return m_animState; };
	HexData* const getAllyHexData() const { return m_allyHexData; };
	HexData* const getEnemyHexData() const { return m_enemyHexData; };
	std::vector<EntityController*>* const getAllies() const { return m_allies; };
	std::vector<EntityController*>* const getEnemies() const { return m_enemies; };
	int const getStackCount(std::string stack);
	std::vector<Stack*>* const getStacks() { return &m_stacks; };
	//std::map<std::string, int>* const getStacks() { return &m_displayStacks; };
	int const getSpawnHex() const { return m_spawnHex; };
	int const getHex() const { return m_hex; };
	std::string const getName() const { return m_name; };
	std::string const getSpecies() const { return m_species; };
	std::string const getCommonName() const { return m_commonName; };
	GLuint const getSprite() const { return m_sprite; };
	GLuint const getIcon() const { return m_icon; };
	template<class T>
	T* getStack() {
		for (auto stack : m_stacks) {
			if (T * stackType = dynamic_cast<T*>(stack)) {
				return stackType;
			}
		}
		return nullptr;
	};

	// ————— SETTERS ————— //
	void const setStat(std::string stat, float val) { m_stats[stat] = val; };
	void const setHealth(float hp) { m_stats["hp"] = hp; };
	void const takeDamage(float damage);
	void const healDamage(float damage);
	void const setSpawnHex(int spawnHex) { m_spawnHex = spawnHex; };
	void const setHex(int hex) { m_hex = hex; };
	void const setSFX(std::map<std::string, Mix_Chunk*> sfx) { m_sfx = sfx; };
	void const setAnimState(AnimState animState);

	template<class T> // T must be a Stack
	T* const addStack(EntityController* source, int count) { 
		T* t = nullptr;
		if (t = getStack<T>()) {
			t->applyStack(count);
		}
		else {
			t = new T(this, source, count);
			m_stacks.push_back(t);
		}
		return t; 
	};
	void const deleteStack(Stack* component);
	void const clearStacks();
};

