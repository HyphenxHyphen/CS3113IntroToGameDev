#include "EntityController.h"
#include "HexController.h"
#include "EntityInfoDisplayController.h"

EntityController::EntityController(GameObject* parent, int id, bool isAnt, std::vector<EntityController*>* allies, std::vector<EntityController*>* enemies, HexData* allyHexData, HexData* enemyHexData, EntityTurnOrderDisplayController* turnOrderDisplay) :Component(parent) {
	m_id = id; 
	m_isAnt = isAnt; 
	m_allies = allies; 
	m_enemies = enemies; 
	m_allyHexData = allyHexData;  
	m_enemyHexData = enemyHexData; 
	m_turnOrderDisplay = turnOrderDisplay;
	build(); 
}

EntityController::EntityController(GameObject* parent, EntityController* original, std::vector<EntityController*>* allies, std::vector<EntityController*>* enemies, HexData* allyHexData, HexData* enemyHexData, EntityTurnOrderDisplayController* turnOrderDisplay) :Component(parent){
	m_id = original->m_id; 
	m_isAnt = original->m_isAnt;
	m_allies = allies; 
	m_enemies = enemies; 
	m_allyHexData = allyHexData;  
	m_enemyHexData = enemyHexData; 
	m_turnOrderDisplay = turnOrderDisplay;
	loadStats(original);
}

void EntityController::getDefault() {
	switch (m_id) {
	case 0:
		m_name = "A610";
		m_species = "ATTA SEXDENS";
		m_commonName = "LEAFCUTTER ANT";
		m_movePool = { 0, 1, 2, 3, 4, 5, 6, 7 };
		m_sprite = Util::loadTexture("Assets/Ants/faraways/A610.png");
		m_icon = Util::loadTexture("Assets/Ants/closeups/A610 closeup.png");
		break;
	case 1:
		m_name = "CASTORIA";
		m_species = "SOLENOPSIS GEMINATA";
		m_commonName = "TOPICAL FIRE ANT";
		m_movePool = { 8, 9, 10, 11, 12, 13, 14, 15 };
		m_sprite = Util::loadTexture("Assets/Ants/faraways/castoria.png");
		m_icon = Util::loadTexture("Assets/Ants/closeups/castoria closeup.png");
		break;
	case 2:
		m_name = "MITZI";
		m_species = "COLOBOPSIS SCHMITZI";
		m_commonName = "DIVING ANT";
		m_movePool = { 16, 17, 18, 19, 20, 21, 22, 23 };
		m_sprite = Util::loadTexture("Assets/Ants/faraways/mitzi.png");
		m_icon = Util::loadTexture("Assets/Ants/closeups/mitzi closeup.png");
		break;
	case 3:
		m_name = "CATERPILLAR";
		m_species = "DANAUS PLEXIPPUS";
		m_commonName = "GREAT MONARCH CATERPILLAR";
		m_movePool = { 23, 24, 25, 26, 27, 28, 29, 30 };
		m_sprite = Util::loadTexture("Assets/Bugs/faraways/great monarch grub.png");
		m_icon = Util::loadTexture("Assets/Bugs/closeups/great monarch grub closeup.png");
		break;
	case 4:
		m_name = "BUTTERFLY";
		m_species = "DANAUS PLEXIPPUS";
		m_commonName = "GREAT MONARCH BUTTERFLY";
		m_movePool = { 23, 24, 25, 26, 27, 28, 29, 30 };
		m_sprite = Util::loadTexture("Assets/Bugs/faraways/great monarch butterfly.png");
		m_icon = Util::loadTexture("Assets/Bugs/closeups/great monarch butterfly closeup.png");
		break;
	case 5:
		m_name = "GRUB";
		m_species = "SCARABAEOIDEA";
		m_commonName = "BEETLE LARVA";
		m_movePool = { 23, 24, 25, 26, 27, 28, 29, 30 };
		m_sprite = Util::loadTexture("Assets/Bugs/faraways/grub.png");
		m_icon = Util::loadTexture("Assets/Bugs/closeups/grub closeup.png");
		break;
	case 6:
		m_name = "SILVERFISH";
		m_species = "LEPISMA SACCHARINUM";
		m_commonName = "SILVERFISH";
		m_movePool = { 23, 24, 25, 26, 27, 28, 29, 30 };
		m_sprite = Util::loadTexture("Assets/Bugs/faraways/silverfish.png");
		m_icon = Util::loadTexture("Assets/Bugs/closeups/silverfish closeup.png");
		break;
	}
}

void EntityController::build() {
	getDefault();

	m_stats["hpMul"] = ((rand() % 6) + 1) + ((rand() % 6) + 1) + ((rand() % 6) + 1) + 6;
	m_stats["atkMul"] = ((rand() % 6) + 1) + ((rand() % 6) + 1) + ((rand() % 6) + 1) + 6;
	m_stats["defMul"] = ((rand() % 6) + 1) + ((rand() % 6) + 1) + ((rand() % 6) + 1) + 6;
	m_stats["accMul"] = ((rand() % 6) + 1) + ((rand() % 6) + 1) + ((rand() % 6) + 1) + 6;
	m_stats["spdMul"] = ((rand() % 6) + 1) + ((rand() % 6) + 1) + ((rand() % 6) + 1) + 6;

	m_stats["dna"] = 25;

	// Placeholder
	m_stats["hpScore"] = m_isAnt ? 5 : 3;
	m_stats["atkScore"] = m_isAnt ? 5 : 3;
	m_stats["defScore"] = m_isAnt ? 5 : 3;
	m_stats["accScore"] = m_isAnt ? 5 : 3;
	m_stats["spdScore"] = m_isAnt ? 5 : 3;

	m_stats["hpMax"] = (float)m_stats["hpScore"] * m_stats["hpMul"];
	m_stats["hp"] = m_stats["hpMax"];
	m_stats["atk"] = (float)m_stats["atkScore"] * m_stats["atkMul"];
	m_stats["def"] = (float)m_stats["defScore"] * m_stats["defMul"];
	m_stats["acc"] = (float)m_stats["accScore"] * m_stats["accMul"];
	m_stats["spd"] = (float)m_stats["spdScore"] * m_stats["spdMul"];

	std::vector<int> movePool = m_movePool;
	for (int i = 0; i < 4; i++) {
		int nextMoveId = rand() % movePool.size();
		m_movesKnown.push_back(UnitMove::getUnitMoveInfo(movePool[nextMoveId]));
		movePool.erase(movePool.begin() + nextMoveId);
	}
}

void EntityController::loadStats(EntityController* other) {
	m_id = other->m_id;
	m_isAnt = other->m_isAnt;

	getDefault();

	m_stats["hpMul"] = other->m_stats["hpMul"];
	m_stats["atkMul"] = other->m_stats["atkMul"];
	m_stats["defMul"] = other->m_stats["defMul"];
	m_stats["accMul"] = other->m_stats["accMul"];
	m_stats["spdMul"] = other->m_stats["spdMul"];

	m_stats["dna"] = 25;

	// Placeholder
	m_stats["hpScore"] = other->m_stats["hpScore"];
	m_stats["atkScore"] = other->m_stats["atkScore"];
	m_stats["defScore"] = other->m_stats["defScore"];
	m_stats["accScore"] = other->m_stats["accScore"];
	m_stats["spdScore"] = other->m_stats["spdScore"];

	m_stats["hpMax"] = (float)m_stats["hpScore"] * m_stats["hpMul"];
	m_stats["hp"] = other->m_stats["hp"];
	m_stats["atk"] = (float)m_stats["atkScore"] * m_stats["atkMul"];
	m_stats["def"] = (float)m_stats["defScore"] * m_stats["defMul"];
	m_stats["acc"] = (float)m_stats["accScore"] * m_stats["accMul"];
	m_stats["spd"] = (float)m_stats["spdScore"] * m_stats["spdMul"];

	m_movePool = other->m_movePool;
	m_movesKnown = other->m_movesKnown;
}

void EntityController::update(float deltaTime) {
	//std::cout << "Updating " << m_name << " is in idle? " << (m_animState == IDLE) << " is ant? " << m_isAnt << std::endl;
	if (m_animState != IDLE) {
		if (m_animState == TURNSTART) {
			if (getPosition().y < m_allyHexData->m_hexes[m_hex]->getPosition().y + 0.7f) {
				setAnimState(IDLE);
			}
		}
		else if (m_animState == TURNEND) {
			setAnimState(IDLE);
			m_turnOrderDisplay->endTurn();
		}
		else if (m_animState == ATTACK) {
			if (((isAnt()) && (getPosition().x < m_allyHexData->m_hexes[m_hex]->getPosition().x)) ||
				((!isAnt()) && (getPosition().x > m_allyHexData->m_hexes[m_hex]->getPosition().x))) {
				setAnimState(TURNEND);
			}
		}
		else if (m_animState == HIT) {
			if (((isAnt()) && (getPosition().x > m_allyHexData->m_hexes[m_hex]->getPosition().x)) ||
				((!isAnt()) && (getPosition().x < m_allyHexData->m_hexes[m_hex]->getPosition().x))) {
				setAnimState(IDLE);
			}
		}
	}
	else if ((!m_isAnt) && (m_turnOrderDisplay->getTurnOrder()->at(0) == this)) {
		// SIMPLE AI
		m_turnWait += deltaTime;
		if (m_turnWait >= 1.0f) {
			UnitMoveInfo randomMove = m_movesKnown[rand() % m_movesKnown.size()];
			EntityController* randomTarget = nullptr;
			if ((randomMove.m_moveRange == SHORT) || (randomMove.m_moveRange == MID) || (randomMove.m_moveRange == LONG)) {
				randomTarget = m_enemies->at(rand() % m_enemies->size());
				while (!randomTarget->m_parent->isAlive()) {
					randomTarget = m_enemies->at(rand() % m_enemies->size());
				}
			}
			else if (randomMove.m_moveRange == ALLY) {
				randomTarget = m_allies->at(rand() % m_allies->size());
				while (!randomTarget->m_parent->isAlive()) {
					randomTarget = m_allies->at(rand() % m_allies->size());
				}
			}
			else if (randomMove.m_moveRange == SELF) {
				randomTarget = this;
			}

			UnitMove::useUnitMove(randomMove.m_moveId, this, randomTarget);
		}
	}
}

void EntityController::updateStats() {
	m_stats["hpMax"] = m_stats["hpScore"] * m_stats["hpMul"] + m_stats["hpMaxDelta"];
	m_stats["atk"] = m_stats["atkScore"] * m_stats["atkMul"] + m_stats["atkDelta"];
	m_stats["def"] = m_stats["defScore"] * m_stats["defMul"] + m_stats["defDelta"];
	m_stats["acc"] = m_stats["accScore"] * m_stats["accMul"] + m_stats["accDelta"];
	m_stats["spd"] = m_stats["spdScore"] * m_stats["spdMul"] + m_stats["spdDelta"];
	m_stats["dmg"] = 1.0f + m_stats["dmgDelta"];
}

void EntityController::diesOfCringe(float deltaTime) {
	m_parent->setAlive(false);
	clearStacks();

	if (m_sfx.count("death")) {
		Mix_PlayChannel(
			-1,       // using the first channel that is not currently in use...
			m_sfx["death"],  // ...play this chunk of audio...
			0        // ...once.
		);
	}
}

void EntityController::onTurnStart() {
	// Do a little hop on turn start, disable buttons while this is active.
	setAnimState(TURNSTART);

	std::vector<int> zeroInds = {};
	for (int i = 0; i < m_stacks.size(); i++) {
		m_stacks[i]->onTurnStart();
		if (m_stacks[i]->m_count <= 0) {
			zeroInds.push_back(i);
		}
	}
	for (int i = zeroInds.size() - 1; i > -1; i--) {
		delete m_stacks[zeroInds[i]];
	}
}

void EntityController::onTurnEnd() {
	std::vector<int> zeroInds = {};
	for (int i = 0; i < m_stacks.size(); i++) {
		m_stacks[i]->onTurnEnd();
		if (m_stacks[i]->m_count <= 0) {
			zeroInds.push_back(i);
		}
	}
	for (int i = zeroInds.size() - 1; i > -1; i--) {
		delete m_stacks[zeroInds[i]];
	}

	m_turnWait = 0.0f;
}

int const EntityController::getStackCount(std::string stackName) {
	int total = 0;
	for (auto& stack : m_stacks) {
		if (stack->m_name.find(stackName) != std::string::npos) {
			total += stack->m_count;
		}
	}
	return total; 
}

void const EntityController::takeDamage(float damage) {
	if ((getStack<StackDoff>() == nullptr) || (getStack<StackDoff>()->m_count <= 0)) {
		if (Stack* block = getStack<StackArmor>()) {
			if (block->m_count >= damage) {
				block->removeStack(damage);
				damage = 0;
			}
			else {
				damage -= block->m_count;
				block->removeStack(block->m_count);
			}
		}
	}

	m_stats["hp"] -= damage;
	if (m_sfx.count("isHit")) {
		Mix_PlayChannel(
			-1,       // using the first channel that is not currently in use...
			m_sfx["isHit"],  // ...play this chunk of audio...
			0        // ...once.
		);
	}
	if (m_stats["hp"] <= 0) {
		diesOfCringe(0);
	}
	else {
		// Reel
		setAnimState(HIT);
	}
}

void const EntityController::healDamage(float damage) {
	m_stats["hp"] += damage;
	if (m_stats["hp"] > m_stats["hpMax"]) {
		m_stats["hp"] = m_stats["hpMax"];
	}
	/*
	if (m_sfx.count("isHit")) {
		Mix_PlayChannel(
			-1,       // using the first channel that is not currently in use...
			m_sfx["isHit"],  // ...play this chunk of audio...
			0        // ...once.
		);
	}
	*/
}

void const EntityController::setAnimState(AnimState animState) {
	m_animState = animState;
	if (m_animState == IDLE) {
			setPosition(X, m_allyHexData->m_hexes[m_hex]->getPosition().x);
			setPosition(Y, m_allyHexData->m_hexes[m_hex]->getPosition().y + 0.7f);
			getComponent<Rigidbody2D>()->setVelocity(X, 0.0f);
			getComponent<Rigidbody2D>()->setAcceleration(X, 0.0f);
			getComponent<Rigidbody2D>()->setVelocity(Y, 0.0f);
			getComponent<Rigidbody2D>()->setAcceleration(Y, 0.0f);
	}
	else if (m_animState == TURNSTART) {
		getComponent<Rigidbody2D>()->setVelocity(Y, 10.0f);
		getComponent<Rigidbody2D>()->setAcceleration(Y, -75.0f);
	}
	else if (m_animState == ATTACK) {
		getComponent<Rigidbody2D>()->setVelocity(X, 10.0f * (isAnt() ? 1.0f : -1.0f));
		getComponent<Rigidbody2D>()->setAcceleration(X, -75.0f * (isAnt() ? 1.0f : -1.0f));
		getComponent<Rigidbody2D>()->setVelocity(Y, 0.0f);
		getComponent<Rigidbody2D>()->setAcceleration(Y, 0.0f);
	}
	else if (m_animState == HIT) {
		getComponent<Rigidbody2D>()->setVelocity(X, -10.0f * (isAnt() ? 1.0f : -1.0f));
		getComponent<Rigidbody2D>()->setAcceleration(X, 75.0f * (isAnt() ? 1.0f : -1.0f));
		getComponent<Rigidbody2D>()->setVelocity(Y, 0.0f);
		getComponent<Rigidbody2D>()->setAcceleration(Y, 0.0f);
	}
}

void const EntityController::deleteStack(Stack* stack) {
	auto it = std::find(m_stacks.begin(), m_stacks.end(), stack);
	if (it != m_stacks.end()) {
		m_stacks.erase(it);
	}
}

void const EntityController::clearStacks() {
	for (Stack* stack : m_stacks) {
		stack->removeStack(stack->m_count);
	}
}