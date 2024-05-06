#include "UnitMove.h"
#include "EntityController.h"
#include "HexController.h"

bool UnitMove::targetUnitMove(UnitMoveInfo move, EntityController* user, EntityController* target) {
	std::vector<EntityController*>* allies = user->getAllies();
	bool targetIsAlly = std::find(allies->begin(), allies->end(), target) != allies->end();

	std::vector<EntityController*>* enemies = user->getEnemies();
	bool targetIsEnemy = std::find(enemies->begin(), enemies->end(), target) != enemies->end();

	if (((move.m_moveRange == SHORT) || (move.m_moveRange == MID) || (move.m_moveRange == LONG)) && (targetIsEnemy)) {
		return true;
	}
	else if ((move.m_moveRange == SELF) && (user == target)) {
		return true;
	}
	else if ((move.m_moveRange == ALLY) && (targetIsAlly)) {
		return true;
	}
	else {
		return false;
	}
}

UnitMoveInfo UnitMove::getUnitMoveInfo(int moveId) {
	switch (moveId) {
	case 0:
		return UnitMoveInfo(
			moveId,
			"LEAF SCALPEL",
			MID,
			"DEAL 60% ATK TO TARGET ENEMY."
		);
		break;
	case 1:
		return UnitMoveInfo(
			moveId,
			"CENTRIFUGE",
			MID,
			"DEAL 40% ATK TO TARGET ENEMY. ROTATE ENEMIES CLOCKWISE 1."
		);
		break;
	case 2:
		return UnitMoveInfo(
			moveId,
			"EXCAVATE",
			MID,
			"ROTATE CLOCKWISE 2. DEAL 40% ATK TO TARGET ENEMY."
		);
	case 3:
		return UnitMoveInfo(
			moveId,
			"DEFOLIATE",
			MID,
			"DEAL 0% ATK TO TARGET ENEMY. DEAL 10% MORE PER STACK OF SPORE ON SELF."
		);
		break;
	case 4:
		return UnitMoveInfo(
			moveId,
			"RED HARVEST",
			ALLY,
			"GRANT SELF 1 RED SPORE. GRANT TARGET 1 RED HARVEST. RED HARVEST: +30% ACC & SPD PER STACK. -1 AT TURN END."
		);
		break;
	case 5:
		return UnitMoveInfo(
			moveId,
			"GREEN HARVEST",
			ALLY,
			"GRANT SELF 1 GREEN SPORE. HEAL TARGET 30% ATK."
		);
		break;
	case 6:
		return UnitMoveInfo(
			moveId,
			"BLUE HARVEST",
			ALLY,
			"GRANT SELF 1 BLUE SPORE. REMOVE 3 DEBUFFS FROM TARGET."
		);
		break;
	case 7:
		return UnitMoveInfo(
			moveId,
			"RAINBOW HARVEST",
			SELF,
			"CONSUME ALL SPORE ON SELF. FOR EACH SPORE, GRANT SELF 1 RAINBOW HARVEST, HEAL 10% ATK, REMOVE 1 DEBUFF FROM SELF. RAINBOW HARVEST: 10% ACC AND SPD PER STACK."
		);
		break;
	case 8:
		return UnitMoveInfo(
			moveId,
			"BURNING STING",
			SHORT,
			"DEAL 100% ATK TO TARGET ENEMY."
		);
		break;
	case 9:
		return UnitMoveInfo(
			moveId,
			"DETONATE",
			SHORT,
			"ACTIVATE ALL SAP ON TARGET."
		);
		break;
	case 10:
		return UnitMoveInfo(
			moveId,
			"IMMOLATE",
			SHORT,
			"APPLY 3 STACKS OF NAPALM SAP TO TARGET ENEMY."
		);
		break;
	case 11:
		return UnitMoveInfo(
			moveId,
			"STUN STING",
			SHORT,
			"DEAL 80% ATK TO TARGET ENEMY. IF TARGET HAS 5 OR MORE STACKS OF NAPALM SAP, CONSUME 5 STACKS AND STUN THEM          FOR 1 TURN."
		);
		break;
	case 12:
		return UnitMoveInfo(
			moveId,
			"BURNING BODY",
			SELF,
			"APPLY 3 STACKS OF NAPALM SAP TO SELF. FOR 3 TURNS, NAPALM SAP HEALS YOU INSTEAD."
		);
		break;
	case 13:
		return UnitMoveInfo(
			moveId,
			"FOCUS FIRE",
			SHORT,
			"TRANSFER ALL STACKS OF NAPALM SAP TO TARGET ENEMY."
		);
		break;
	case 14:
		return UnitMoveInfo(
			moveId,
			"HELL STING",
			SHORT,
			"DEAL 140% ATK TO TARGET ENEMY. APPLY 1 STACK OF NAPALM SAP TO ALLIES."
		);
		break;
	case 15:
		return UnitMoveInfo(
			moveId,
			"FLAME WHEEL",
			SELF,
			"ROTATE CLOCKWISE UNTIL I AM IN POSITION 2. APPLY A STACK OF NAPALM SAP TO SELF PER HEX MOVED."
		);
		break;
	case 16:
		return UnitMoveInfo(
			moveId,
			"SALVAGE LINE",
			MID,
			"DEAL 80% ATK TO TARGET ENEMY. ROTATE ENEMIES COUNTERCLOCKWISE 1."
		);
		break;
	case 17:
		return UnitMoveInfo(
			moveId,
			"SWEET LURE",
			LONG,
			"ROTATE ENEMIES COUNTERCLOCKWISE 2."
		);
		break;
	case 18:
		return UnitMoveInfo(
			moveId,
			"PLANT DEFENSE",
			SELF,
			"GRANT SELF 20 ARMOR. GAIN 20% DEF UNTIL MY NEXT TURN."
		);
		break;
	case 19:
		return UnitMoveInfo(
			moveId,
			"STICKY BODY",
			SHORT,
			"STUN TARGET FOR 1 TURN."
		);
		break;
	case 20:
		return UnitMoveInfo(
			moveId,
			"DOFF ARMOR",
			SELF,
			"GRANT SELF 1 DOFF. WHILE I HAVE DOFF, ARMOR DOES NOT PROTECT ME. INCREASE MY DAMAGE BY 1% PER STACK OF ARMOR PER STACK OF DOFF."
		);
		break;
	case 21:
		return UnitMoveInfo(
			moveId,
			"DIVE BOMB",
			SHORT,
			"DEAL 100% ATK TO TARGET ENEMY, INCREASED BY 1% PER ARMOR. LOSE ALL ARMOR."
		);
		break;
	case 22:
		return UnitMoveInfo(
			moveId,
			"RAIN SHELTER",
			ALLY,
			"GAIN 10 ARMOR. TRANSFER ALL STACKS OF ARMOR TO TARGET."
		);
		break;
	case 23:
		return UnitMoveInfo(
			moveId,
			"HEALING JUICE",
			SELF,
			"HEAL 60% ATK TO SELF."
		);
		break;
	case 24:
		return UnitMoveInfo(
			moveId,
			"FEAST",
			SHORT,
			"DEAL 20% ATK TO TARGET ENEMY. HEAL SELF 20% ATK. GRANT SELF 1 STACK OF GROWTH."
		);
		break;
	case 25:
		return UnitMoveInfo(
			moveId,
			"GROW",
			SELF,
			"GAIN 30% DEF UNTIL START OF MY NEXT TURN. GRANT SELF 2 STACKS OF GROWTH."
		);
		break;
	case 26:
		return UnitMoveInfo(
			moveId,
			"MILKWEED SPIT",
			MID,
			"DEAL 20% ATK TO TARGET ENEMY. GRANT TARGET 1 STACK OF POISONED. POISONED TARGETS LOSE 10% HPMAX ON TURN START PER STACK."
		);
		break;
	case 27:
		return UnitMoveInfo(
			moveId,
			"NIBBLE",
			SHORT,
			"DEAL 50% ATK TO TARGET ENEMY."
		);
		break;
	case 28:
		return UnitMoveInfo(
			moveId,
			"NAP",
			SELF,
			"HEAL 50% ATK TO SELF."
		);
		break;
	case 29:
		return UnitMoveInfo(
			moveId,
			"CAUGHT SPIRACLES",
			SHORT,
			"DEAL 75% ATK TO TARGET ENEMY. DEAL 25% ATK TO SELF."
		);
		break;
	case 30:
		return UnitMoveInfo(
			moveId,
			"WRIGGLE",
			SELF,
			"GAIN 10% SPD UNTIL NEXT TURN END."
		);
		break;
	}
}

void UnitMove::useUnitMove(int moveId, EntityController* user, EntityController* target) {
	if (moveId == 0) {
		std::cout << "Used LEAF SCALPEL" << std::endl;
		user->setAnimState(ATTACK);
		std::cout << "my dmg mul: " << user->getStat("dmg") << std::endl;
		target->takeDamage(0.6f * user->getStat("atk") * user->getStat("dmg"));
	}
	else if (moveId == 1) {
		std::cout << "Used CENTRIFUGE" << std::endl;
		user->setAnimState(ATTACK);
		target->takeDamage(0.4f * user->getStat("atk") * user->getStat("dmg"));
		HexController::rotateHexes(user->getEnemyHexData(), true, 1);
	}
	else if (moveId == 2) {
		std::cout << "Used EXCAVATE" << std::endl;
		HexController::rotateHexes(user->getAllyHexData(), true, 2);
		user->setAnimState(ATTACK);
		target->takeDamage(0.4f * user->getStat("atk") * user->getStat("dmg"));
	}
	else if (moveId == 3) {
		std::cout << "Used DEFOLIATE" << std::endl;
		user->setAnimState(ATTACK);
		target->takeDamage(0.1f * user->getStackCount("SPORE") * user->getStat("atk"));
	}
	else if (moveId == 4) {
		std::cout << "Used RED HARVEST" << std::endl;
		user->addStack<StackRedSpore>(user, 1);
		if ((target == user) && (user->getStackCount("RED HARVEST") == 0)) {
			target->addStack<StackRedHarvest>(user, 2);
		}
		else {
			target->addStack<StackRedHarvest>(user, 1);
		}
		user->setAnimState(TURNEND);
	}
	else if (moveId == 5) {
		std::cout << "Used GREEN HARVEST" << std::endl;
		user->addStack<StackGreenSpore>(user, 1);
		target->healDamage(0.3f * user->getStat("atk"));
		user->setAnimState(TURNEND);
	}
	else if (moveId == 6) {
		std::cout << "Used BLUE HARVEST" << std::endl;
		user->addStack<StackBlueSpore>(user, 1);
		std::vector<Stack*> debuffs = {};
		int debuffCount = 0;
		for (auto& stack : *target->getStacks()) {
			if (!stack->m_buff) {
				debuffCount += stack->m_count;
				debuffs.push_back(stack);
			}
		}
		int removeMax = 3;
		if (debuffCount < removeMax) {
			removeMax = debuffs.size();
		}
		for (int i = 0; i < removeMax; i++) {
			debuffs[rand() % debuffs.size()]->removeStack(1);
		}
		user->setAnimState(TURNEND);
	}
	else if (moveId == 7) {
		std::cout << "Used RAINBOW HARVEST" << std::endl;

		// RAINBOW COUNT
		std::vector<Stack*> spores = {};
		for (auto& stack : *user->getStacks()) {
			if (stack->m_name.find("SPORE") != std::string::npos) {
				spores.push_back(stack);
			}
		}
		int sporeSum = 0;
		for (int i = 0; i < spores.size(); i++) {
			sporeSum += spores[i]->m_count;
			spores[i]->removeStack(spores[i]->m_count);
		}

		// RED
		if (user->getStackCount("RAINBOW HARVEST") == 0) {
			user->addStack<StackRainbowHarvest>(user, sporeSum + 1);
		}
		else {
			user->addStack<StackRainbowHarvest>(user, sporeSum);
		}

		// GREEN
		user->healDamage(sporeSum * 0.1f * user->getStat("atk"));

		// BLUE
		std::vector<Stack*> debuffs = {};
		int debuffCount = 0;
		for (auto& stack : *user->getStacks()) {
			if (!stack->m_buff) {
				debuffCount += stack->m_count;
				debuffs.push_back(stack);
			}
		}
		int removeMax = sporeSum;
		if (debuffCount < removeMax) {
			removeMax = debuffs.size();
		}
		for (int i = 0; i < removeMax; i++) {
			debuffs[rand() % debuffs.size()]->removeStack(1);
		}
		user->setAnimState(TURNEND);
	}
	else if (moveId == 8) {
		std::cout << "Used BURNING STING" << std::endl;
		user->setAnimState(ATTACK);
		target->takeDamage(1.0f * user->getStat("atk") * user->getStat("dmg"));
	}
	else if (moveId == 9) {
		std::cout << "Used DETONATE" << std::endl;
		user->setAnimState(ATTACK);
		for (auto& stack : *target->getStacks()) {
			if (stack->m_name.find("SAP") != std::string::npos) {
				stack->onTurnStart();
				stack->onTurnEnd();
			}
		}
	}
	else if (moveId == 10) {
		std::cout << "Used IMMOLATE" << std::endl;
		user->setAnimState(ATTACK);
		target->addStack<StackNapalmSap>(user, 3);
	}
	else if (moveId == 11) {
		std::cout << "Used STUN STING" << std::endl;
		user->setAnimState(ATTACK);
		target->takeDamage(0.8f * user->getStat("atk") * user->getStat("dmg"));
		if (Stack * napalm = target->getStack<StackNapalmSap>()) {
			if (napalm->m_count >= 5) {
				napalm->removeStack(5);
				target->addStack<StackStunned>(user, 1);
			}
		}
	}
	else if (moveId == 12) {
		std::cout << "Used BURNING BODY" << std::endl;
		target->addStack<StackNapalmSap>(user, 3);
		target->addStack<StackBurningBody>(user, 3);
		user->setAnimState(TURNEND);
	}
	else if (moveId == 13) {
		std::cout << "Used FOCUS FIRE" << std::endl;
		user->setAnimState(ATTACK);
		int napalmCount = 0;
		for (auto& ally : *user->getAllies()) {
			if (StackNapalmSap * napalm = ally->getStack<StackNapalmSap>()) {
				if (napalm->m_count > 0) {
					napalmCount += napalm->m_count;
					napalm->removeStack(napalm->m_count);
				}
			}
		}
		for (auto& enemy : *user->getEnemies()) {
			if (StackNapalmSap * napalm = enemy->getStack<StackNapalmSap>()) {
				if (napalm->m_count > 0) {
					napalmCount += napalm->m_count;
					napalm->removeStack(napalm->m_count);
				}
			}
		}
		target->addStack<StackNapalmSap>(user, napalmCount);
	}
	else if (moveId == 14) {
		std::cout << "Used HELL STING" << std::endl;
		user->setAnimState(ATTACK);
		target->takeDamage(1.4f * user->getStat("atk") * user->getStat("dmg"));
		for (auto& ally : *user->getAllies()) {
			ally->addStack<StackNapalmSap>(user, 1);
		}
	}
	else if (moveId == 15) {
		std::cout << "Used FLAME WHEEL" << std::endl;
		int rotations = 0;
		while (user->getHex() != 1) {
			HexController::rotateHexes(user->getAllyHexData(), true, 1);
			rotations++;
		}
		user->addStack<StackNapalmSap>(user, rotations);
		user->setAnimState(TURNEND);
	}
	else if (moveId == 16) {
		std::cout << "Used SALVAGE LINE" << std::endl;
		user->setAnimState(ATTACK);
		target->takeDamage(0.8f * user->getStat("atk") * user->getStat("dmg"));
		HexController::rotateHexes(user->getEnemyHexData(), false, 1);
	}
	else if (moveId == 17) {
		std::cout << "Used SWEET LURE" << std::endl;
		user->setAnimState(ATTACK);
		HexController::rotateHexes(user->getEnemyHexData(), false, 2);
	}
	else if (moveId == 18) {
		std::cout << "Used PLANT DEFENSE" << std::endl;
		user->addStack<StackArmor>(user, 20);
		user->addStack<StackPlantDefense>(user, 1);
		user->setAnimState(TURNEND);
	}
	else if (moveId == 19) {
		std::cout << "Used STICKY BODY" << std::endl;
		user->setAnimState(ATTACK);
		target->addStack<StackStunned>(user, 1);
	}
	else if (moveId == 20) {
		std::cout << "Used DOFF ARMOR" << std::endl;
		user->addStack<StackDoff>(user, 1);
		user->setAnimState(TURNEND);
	}
	else if (moveId == 21) {
		std::cout << "Used DIVE BOMB" << std::endl;
		user->setAnimState(ATTACK);
		float armorDmgMul = 1.0f;
		if (Stack* armor = user->getStack<StackArmor>()) {
			armorDmgMul += armor->m_count * 0.01f;
		}
		target->takeDamage(1.0f * user->getStat("atk") * user->getStat("dmg") * armorDmgMul);
		if (Stack* armor = user->getStack<StackArmor>()) {
			armor->removeStack(armor->m_count);
		}
	}
	else if (moveId == 22) {
		std::cout << "Used RAIN SHELTER" << std::endl;
		user->addStack<StackArmor>(user, 10);
		int armorCount = 0;
		for (auto& ally : *user->getAllies()) {
			if (StackArmor* armor = ally->getStack<StackArmor>()) {
				if (armor->m_count > 0) {
					armorCount += armor->m_count;
					armor->removeStack(armor->m_count);
				}
			}
		}
		for (auto& enemy : *user->getEnemies()) {
			if (StackArmor* armor = enemy->getStack<StackArmor>()) {
				if (armor->m_count > 0) {
					armorCount += armor->m_count;
					armor->removeStack(armor->m_count);
				}
			}
		}
		target->addStack<StackArmor>(user, armorCount);
		user->setAnimState(TURNEND);
	}
	else if (moveId == 23) {
		std::cout << "Used HEALING JUICE" << std::endl;
		target->healDamage(0.6f * user->getStat("atk"));
		user->setAnimState(TURNEND);
	}
	else if (moveId == 24) {
	std::cout << "Used FEAST" << std::endl;
	target->takeDamage(0.2f * user->getStat("atk") * user->getStat("dmg"));
	user->healDamage(0.2f * user->getStat("atk"));
	user->addStack<StackGrowth>(user, 1);
	user->setAnimState(ATTACK);
	}
	else if (moveId == 25) {
	std::cout << "Used GROW" << std::endl;
	user->addStack<StackGrowDefense>(user, 1);
	user->addStack<StackGrowth>(user, 2);
	user->setAnimState(TURNEND);
	}
	else if (moveId == 26) {
	std::cout << "Used MILKWEED SPIT" << std::endl;
	target->takeDamage(0.2f * user->getStat("atk") * user->getStat("dmg"));
	target->addStack<StackPoisoned>(user, 1);
	user->setAnimState(ATTACK);
	}
	else if (moveId == 27) {
	std::cout << "Used NIBBLE" << std::endl;
	target->takeDamage(0.5f * user->getStat("atk") * user->getStat("dmg"));
	user->setAnimState(ATTACK);
	}
	else if (moveId == 28) {
	std::cout << "Used NAP" << std::endl;
	user->healDamage(0.5f * user->getStat("atk"));
	user->setAnimState(TURNEND);
	}
	else if (moveId == 29) {
	std::cout << "Used CAUGHT SPIRACLES" << std::endl;
	target->takeDamage(0.75f * user->getStat("atk") * user->getStat("dmg"));
	user->takeDamage(0.25f * user->getStat("atk") * user->getStat("dmg"));
	user->setAnimState(TURNEND);
	}
	else if (moveId == 30) {
	std::cout << "Used WRIGGLE" << std::endl;
	user->addStack<StackWriggle>(user, 2);
	user->setAnimState(TURNEND);
	}
}